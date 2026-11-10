import SwiftUI

struct CalibrationSection: View {
    @Bindable var boat: BoatControl

    var body: some View {
        Section {
            calibrationField("Neutro (µs)", value: $boat.calibration.stop)
            calibrationField("Hacia 0° (ms)", value: $boat.calibration.toZero)
            calibrationField("Hacia 180° (ms)", value: $boat.calibration.to180)
            Group {
                Button("Probar neutro durante 1,5 s") { boat.test(direction: 1, duration: 1500) }
                Button("Probar giro hacia 0°") { boat.test(direction: 0, duration: boat.calibration.toZero) }
                Button("Probar giro hacia 180°") { boat.test(direction: 2, duration: boat.calibration.to180) }
                Toggle("Verifiqué el neutro y medí 90° en ambos sentidos", isOn: $boat.confirmed)
                Button("Guardar calibración") { boat.saveCalibration() }
                    .disabled(!boat.confirmed)
                Button("Pulso hacia 0° · 40 ms") { boat.test(direction: 0, duration: 40) }
                Button("Pulso hacia 180° · 40 ms") { boat.test(direction: 2, duration: 40) }
            }
            .disabled(boat.calibration.parameters == nil)
        } header: {
            Text("Calibración del servo")
        } footer: {
            Text("Usa un neutro entre 1300 y 1700 µs y tiempos entre 20 y 5000 ms. Haz las pruebas con el eje libre. Usa los pulsos cortos para alinear el centro.")
        }
        .disabled(!boat.canConfigure)
    }

    private func calibrationField(_ title: String, value: Binding<Int?>) -> some View {
        LabeledContent {
            TextField(title, value: value, format: .number.grouping(.never), prompt: Text("—"))
                .keyboardType(.numberPad)
                .multilineTextAlignment(.trailing)
        } label: {
            Text(title)
        }
    }
}
