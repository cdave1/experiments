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

DispatchQueue.global(qos: .default).async {
    var oscillator = AKOscillator()
    var oscillator2 = AKOscillator()

    let MIDDLE_C = 261.63

    oscillator.frequency = MIDDLE_C * 1.0
    oscillator.amplitude = 1.0

    oscillator2.frequency = MIDDLE_C * 4.0
    oscillator2.amplitude = 0.5

    let envelope = AKAmplitudeEnvelope(oscillator)
    envelope.attackDuration = 0.01
    envelope.decayDuration = 0.1
    envelope.sustainLevel = 0.1
    envelope.releaseDuration = 0.3

    let timer = Timer(timeInterval: 1.0, repeats: true) { timer in
        //oscillator.frequency = oscillator.frequency == MIDDLE_C * 0.5 ? MIDDLE_C * 1.0 : MIDDLE_C * 1.0
    }

    let timer2 = Timer(timeInterval: 0.5, repeats: true) { timer in
        oscillator2.frequency = oscillator2.frequency == MIDDLE_C * 3.0 ? MIDDLE_C * 2.0 : MIDDLE_C * 3.0
    }

    let timer3 = Timer(timeInterval: 0.5, repeats: true) { timer in
        if (envelope.isStarted) {
            envelope.stop()
        } else {
            envelope.start()
        }
    }

    RunLoop.main.add(timer, forMode: .common)
    RunLoop.main.add(timer2, forMode: .common)
    RunLoop.main.add(timer3, forMode: .common)

    do {
        AudioKit.output = AKMixer(envelope, oscillator2)
        try AudioKit.start()
        oscillator.start()
        oscillator2.start()
        envelope.start()
    } catch {
        print("Error: \(error)")
    }
    //group.leave()
}

group.notify(queue: .main, execute: {
    print("DONE")
})

var shouldKeepRunning = true
let theRL = RunLoop.current
while shouldKeepRunning && RunLoop.current.run(mode: RunLoop.Mode.default, before: .distantFuture) { }

print("Starting")
group.wait()
//


