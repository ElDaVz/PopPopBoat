struct Calibration: Equatable {
    var stop: Int? = 1500
    var toZero: Int? = 200
    var to180: Int? = 200

    var parameters: [String: Int]? {
        guard let stop, (1300...1700).contains(stop),
              let toZero, (20...5000).contains(toZero),
              let to180, (20...5000).contains(to180) else { return nil }
        return ["stop_us": stop, "to_zero_ms": toZero, "to_180_ms": to180]
    }
}
