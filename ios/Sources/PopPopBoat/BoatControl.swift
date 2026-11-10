import Observation

@MainActor
@Observable
final class BoatControl {
    private let api: BoatAPI
    private var revision = 0
    private var commandInFlight = false
    private var stopRequest: Task<Void, Never>?
    private var online = false

    private(set) var status: BoatStatus?
    private(set) var errorMessage: String?
    var calibration = Calibration() {
        didSet {
            if calibration != oldValue { confirmed = false }
        }
    }
    var confirmed = false

    init(api: BoatAPI = BoatAPI()) {
        self.api = api
    }

    var connectionLabel: String {
        if online { return "Conectado a PopPopBoat" }
        return errorMessage == nil ? "Buscando el barco…" : "Sin conexión"
    }

    var canConfigure: Bool {
        online && !commandInFlight && stopRequest == nil && status?.moving == false
    }

    var canStop: Bool {
        stopRequest == nil && status != nil
    }

    func monitor() async {
        while !Task.isCancelled {
            if !commandInFlight && stopRequest == nil {
                let currentRevision = revision
                do {
                    let next = try await api.status()
                    guard !Task.isCancelled else { return }
                    if currentRevision == revision { updateStatus(next) }
                } catch {
                    guard !Task.isCancelled else { return }
                    if currentRevision == revision, online || errorMessage == nil {
                        report(error)
                    }
                }
            }
            let interval: Duration = status?.moving == true && online ? .milliseconds(200) : .seconds(1)
            do {
                try await Task.sleep(for: interval)
            } catch {
                return
            }
        }
    }

    func move(to angle: Int) {
        guard status?.referenced == true, status?.calibrated == true else { return }
        send("angle", parameters: ["value": angle])
    }

    func reference() {
        guard status?.calibrated == true else { return }
        send("reference")
    }

    func test(direction: Int, duration: Int?) {
        guard let duration, (20...5000).contains(duration),
              var parameters = calibration.parameters else { return }
        parameters["direction"] = direction
        parameters["duration_ms"] = duration
        send("test", parameters: parameters)
    }

    func saveCalibration() {
        guard confirmed, var parameters = calibration.parameters else { return }
        parameters["confirmed"] = 1
        send("calibration", parameters: parameters)
    }

    func stop() {
        guard canStop else { return }
        revision += 1
        startStop()
    }

    private func send(_ path: String, parameters: [String: Int] = [:]) {
        guard canConfigure else { return }
        revision += 1
        let commandRevision = revision
        commandInFlight = true
        errorMessage = nil
        Task {
            defer { commandInFlight = false }
            do {
                let next = try await api.command(path, parameters: parameters)
                if commandRevision == revision {
                    updateStatus(next)
                    if path == "calibration" { confirmed = false }
                }
            } catch {
                if commandRevision == revision { report(error) }
            }
            // A pending move may have reached the board after the first stop.
            if commandRevision != revision {
                await stopRequest?.value
                if stopRequest == nil { startStop() }
                await stopRequest?.value
            }
        }
    }

    private func startStop() {
        stopRequest = Task {
            defer { stopRequest = nil }
            do {
                let next = try await api.command("stop")
                errorMessage = nil
                updateStatus(next)
            } catch {
                report(error)
            }
        }
    }

    private func updateStatus(_ next: BoatStatus) {
        if status == nil || next.uptimeS < (status?.uptimeS ?? 0) {
            calibration = Calibration(stop: next.stopUs, toZero: next.toZeroMs, to180: next.to180Ms)
            confirmed = false
        }
        if !online { errorMessage = nil }
        online = true
        status = next
    }

    private func report(_ error: any Error) {
        if let rejection = error as? RejectedCommand {
            errorMessage = rejection.message
        } else {
            online = false
            errorMessage = "Conéctate al Wi-Fi PopPopBoat en Ajustes y permite el acceso a la red local."
        }
    }
}
