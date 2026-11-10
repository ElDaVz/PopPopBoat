import Foundation
#if canImport(FoundationNetworking)
import FoundationNetworking
#endif

struct BoatAPI: Sendable {
    private let baseURL: URL
    private let session: URLSession

    init(baseURL: URL = URL(string: "http://192.168.4.1")!) {
        self.baseURL = baseURL
        let configuration = URLSessionConfiguration.ephemeral
        configuration.requestCachePolicy = .reloadIgnoringLocalCacheData
        configuration.timeoutIntervalForRequest = 2
        configuration.timeoutIntervalForResource = 3
        session = URLSession(configuration: configuration)
    }

    func status() async throws -> BoatStatus {
        try await request("status", method: "GET", parameters: [:])
    }

    func command(_ path: String, parameters: [String: Int] = [:]) async throws -> BoatStatus {
        try await request(path, method: "POST", parameters: parameters)
    }

    private func request(_ path: String, method: String, parameters: [String: Int]) async throws -> BoatStatus {
        let endpoint = baseURL.appending(path: "api").appending(path: path)
        guard var components = URLComponents(url: endpoint, resolvingAgainstBaseURL: false) else {
            throw URLError(.badURL)
        }
        if !parameters.isEmpty {
            components.queryItems = parameters.map { URLQueryItem(name: $0.key, value: String($0.value)) }
        }
        guard let url = components.url else { throw URLError(.badURL) }

        var request = URLRequest(url: url)
        request.httpMethod = method
        let (data, response) = try await session.data(for: request)
        guard let http = response as? HTTPURLResponse else { throw URLError(.badServerResponse) }
        guard (200..<300).contains(http.statusCode) else {
            throw RejectedCommand(message: String(decoding: data, as: UTF8.self))
        }
        let decoder = JSONDecoder()
        decoder.keyDecodingStrategy = .convertFromSnakeCase
        return try decoder.decode(BoatStatus.self, from: data)
    }
}
