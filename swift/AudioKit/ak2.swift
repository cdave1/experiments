//
//  main.swift
//  BasicMusic
//
//  Created by David Petrie on 8/09/19.
//  Copyright © 2019 David Petrie. All rights reserved.
//

import Foundation
import AudioKit

let group = DispatchGroup()
group.enter()

func createAndStartOscillator(frequency: Double, amplitude: Double) -> AKPhaseDistortionOscillator {
    var osc = AKPhaseDistortionOscillator()
    osc.frequency = frequency
    osc.amplitude = amplitude
    osc.phaseDistortion = 1.0
    osc.start()
    return osc
}

func createAndStartTimedOscillator(frequency:Double, amplitude: Double, bpm: UInt, firstBeat: UInt, repeats: UInt, len: UInt) -> AKPhaseDistortionOscillator {
    let osc = createAndStartOscillator(frequency: frequency, amplitude: amplitude)

    let _bpm = (1 ... 10000).clamp(bpm)

    // Convert bpm to a time interval
    let BPS:UInt = 16
    let interval = 1.0 / 16.0
    var _nextBeat:UInt = firstBeat
    var currentBeat:UInt = 1
    var _stopBeat:UInt = 0;

    let timer = Timer(timeInterval: interval, repeats: true) { timer in
        if (currentBeat > BPS) {
            _nextBeat = firstBeat
            currentBeat = 1
        }

        osc.phaseDistortion = -1.0 + (Double(currentBeat) / 16.0) * 2.0

        if (currentBeat == _nextBeat) {
            osc.start()
            _stopBeat = _nextBeat + len
            _nextBeat = repeats + currentBeat
        }
        if (currentBeat == _stopBeat) {
            osc.stop()
        }
        currentBeat += 1
    }

    RunLoop.main.add(timer, forMode: .common)
    return osc
}

DispatchQueue.global(qos: .default).async {
    let MIDDLE_C = 261.63

    var o1 = createAndStartOscillator(frequency: MIDDLE_C * 1.0, amplitude: 1.0)
    var o2 = createAndStartOscillator(frequency: MIDDLE_C * 4.0, amplitude: 1.0)

    var b0 = createAndStartTimedOscillator(frequency: MIDDLE_C * 0.25, amplitude: 1.0, bpm: 120, firstBeat: 3, repeats: 8, len: 4)
    var b1 = createAndStartTimedOscillator(frequency: MIDDLE_C * 0.5, amplitude: 1.0, bpm: 120, firstBeat: 1, repeats: 8, len: 1)
    var b2 = createAndStartTimedOscillator(frequency: MIDDLE_C * 1.0, amplitude: 1.0, bpm: 120, firstBeat: 5, repeats: 0, len: 2)
    var b3 = createAndStartTimedOscillator(frequency: MIDDLE_C * 8.0, amplitude: 0.5, bpm: 120, firstBeat: 3, repeats: 6, len: 1)
    var b4 = createAndStartTimedOscillator(frequency: MIDDLE_C * 2.0, amplitude: 0.5, bpm: 120, firstBeat: 7, repeats: 4, len: 1)
    var b5 = createAndStartTimedOscillator(frequency: MIDDLE_C * 12.0, amplitude: 0.5, bpm: 120, firstBeat: 13, repeats: 0, len: 1)

    let mixer1 = AKMixer(o1, o2)
    //mixer.connect(o1)
    //mixer.connect(o2)
    mixer1.connect(b0)
    mixer1.connect(b1)
    mixer1.connect(b2)
    mixer1.connect(b3)
    mixer1.connect(b4)
    mixer1.connect(b5)

    let clarinet = AKClarinet(frequency: MIDDLE_C, amplitude: 1.0);
    clarinet.start();
    mixer1.connect(clarinet)

    var rev = AKReverb(mixer1)
    rev.start()

    let mixerMain = AKMixer(mixer1, rev)

    do {
        AudioKit.output = mixerMain
        try AudioKit.start()
    } catch {
        print("Error: \(error)")
    }
}

group.notify(queue: .main, execute: {
    print("DONE")
})

var shouldKeepRunning = true
let theRL = RunLoop.current
while shouldKeepRunning && RunLoop.current.run(mode: RunLoop.Mode.default, before: .distantFuture) { }

print("Starting")
group.wait()
