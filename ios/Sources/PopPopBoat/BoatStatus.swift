struct BoatStatus: Decodable {
    let angle: Int
    let target: Int
    let referenced: Bool
    let moving: Bool
    let testing: Bool
    let calibrated: Bool
    let remainingMs: Int
    let stopUs: Int
    let toZeroMs: Int
    let to180Ms: Int
    let uptimeS: Int
}
