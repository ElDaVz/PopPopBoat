import SwiftUI

struct ControlView: View {
    @State private var boat = BoatControl()

    var body: some View {
        NavigationStack {
            Form {
                Section("Conexión") {
                    Label(boat.connectionLabel, systemImage: "wifi")
                    if let error = boat.errorMessage {
                        Text(error)
                            .font(.footnote)
                            .foregroundStyle(.secondary)
                    }
                }
                Section {
                    if let status = boat.status, status.referenced {
                        LabeledContent("Posición estimada", value: "\(status.angle)°")
                    }
                    Text(rudderStatus)
                        .foregroundStyle(.secondary)
                    ViewThatFits(in: .horizontal) {
                        HStack { presets }
                        VStack(alignment: .leading) { presets }
                    }
                    .disabled(!boat.canConfigure || boat.status?.referenced != true)

                    Button("Marcar centro en 90°", systemImage: "scope") {
                        boat.reference()
                    }
                    .disabled(!boat.canConfigure || boat.status?.calibrated != true)
                } header: {
                    Text("Timón")
                } footer: {
                    Text("Alinea el timón antes de marcar el centro. La posición se estima por tiempo y puede desviarse.")
                }
                CalibrationSection(boat: boat)
            }
            .scrollDismissesKeyboard(.interactively)
            .safeAreaInset(edge: .bottom) {
                Button("Detener movimiento", systemImage: "stop.fill", role: .destructive) {
                    boat.stop()
                }
                .buttonStyle(.borderedProminent)
                .controlSize(.large)
                .disabled(!boat.canStop)
                .frame(maxWidth: .infinity)
                .padding()
                .background(.bar)
            }
            .navigationTitle("PopPopBoat")
        }
        .task { await boat.monitor() }
    }

    private var rudderStatus: String {
        guard let status = boat.status else { return "Esperando el estado del timón." }
        if status.testing { return "Prueba en curso." }
        if status.moving { return "Girando hacia \(status.target)°." }
        if !status.calibrated { return "Calibra el servo antes de mover el timón." }
        if !status.referenced { return "Alinea el timón y marca el centro." }
        return "Listo para girar."
    }

    private var presets: some View {
        ForEach([0, 90, 180], id: \.self) { angle in
            Button("\(angle)°") { boat.move(to: angle) }
                .buttonStyle(.bordered)
                .controlSize(.large)
                .accessibilityLabel("Mover a \(angle) grados")
        }
    }
}
