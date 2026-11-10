// swift-tools-version: 6.4
import PackageDescription

let package = Package(
    name: "PopPopBoat",
    platforms: [.iOS(.v17)],
    products: [.library(name: "PopPopBoat", targets: ["PopPopBoat"])],
    targets: [.target(name: "PopPopBoat")]
)
