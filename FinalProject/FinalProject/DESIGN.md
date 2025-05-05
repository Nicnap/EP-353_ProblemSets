# WizardWave User Manual
## Overview

This document describes the architecture and design of the **WizardWave** audio plugin, built with JUCE. :contentReference[oaicite:0]{index=0} It covers the synthesiser setup, DSP effects chain, UI layout, parameter mapping, and code structure. I will also explain my decisions. :contentReference[oaicite:1]{index=1}

## 1. Plugin Architecture

I used`juce::Synthesiser` to manage voice allocation and polyphony, following the “Build a MIDI synthesiser” JUCE tutorial. :contentReference[oaicite:2]{index=2} Each of five `SynthVoice` instances inherits from `juce::SynthesiserVoice` and implements `startNote`, `stopNote`, and `renderNextBlock` to create waveforms. :contentReference[oaicite:3]{index=3}


Next I used`juce::dsp::ProcessorChain` to sequence the Reverb, LPF, HPF, and Distortion blocks. :contentReference[oaicite:5]{index=5} This is the pattern used in the Intro to DSP tutorial provided by JUCE to keep per‑block processing safe and efficient. :contentReference[oaicite:6]{index=6} Each processor is prepared via `prepare(spec)` to match the host’s sample rate and block size. :contentReference[oaicite:7]{index=7}

## 2. Wave Morphing and ADSR

Now I needed a helper `generateWaveSample()` that interpolates between sine, triangle, pulse, and saw shapes based on an index. :contentReference[oaicite:8]{index=8} This approach allows continuous “morphing” by adjusting the mix parameter inside each `renderNextBlock` letting the player change the wave in real time. :contentReference[oaicite:9]{index=9}


Now that it was continuous, I needed`juce::ADSR` per voice for amplitude shaping. :contentReference[oaicite:10]{index=10} ADSR parameters (attack, decay, sustain, release) are exposed via a toggle, following JUCE tutorials on correct ADSR integration into DSP chains. At the present moment, the ADSR can only be edited internally to the code. :contentReference[oaicite:11]{index=11}

## 3. UI & Interaction Design


The UI presents five points arranged in a pentagon. :contentReference[oaicite:12]{index=12} Users click to connect dots; externally dot connections select waveform type (0=none, 1=sine, …, 5=saw). Internal connections select DSP effects. :contentReference[oaicite:13]{index=13} Right‑click on a line removes it. :contentReference[oaicite:14]{index=14}

An `juce::Slider` at the bottom controls “Intensity” (0–100) that modulates effect parameters. :contentReference[oaicite:15]{index=15}. It's placed below the pentagon to avoid UI clutter, using JUCE’s `Slider` API. :contentReference[oaicite:16]{index=16}

## 4. Effect Parameter Mapping

- **Reverb**: Room size ranges from 0.2 to 1.0 based on intensity, giving shorter to longer decay. :contentReference[oaicite:17]{index=17}  
- **Delay**: Delay time mapped from 1/16 to 1/2 note at 120 BPM ONLY (I could not figure out how to map this to DAW tempo in time) via `jmap<double>`, ensuring type safety. :contentReference[oaicite:18]{index=18}  
- **LPF/HPF**: Cutoff updated by reassigning IIR coefficients with `makeLowPass`/`makeHighPass`, since `setCutOffFrequency` is not exposed. :contentReference[oaicite:19]{index=19}  
- **Distortion**: A simple `tanh` waveshaper whose drive is scaled by intensity. :contentReference[oaicite:20]{index=20}

## 5. Key Design Decisions

- **ProcessorChain**: Enforces correct initialization and avoids manual memory errors that can crash hosts. :contentReference[oaicite:30]{index=30}  
- **Intensity Parameter**: Provides a single control to modulate multiple effects, demonstrating real‑time parameter mapping. I wanted the synth to feel like casting a spell. Like you were in control but it has something of a mind of its own. By mapping all these parameters to the same meter and not labelling what EXACTLY it does, I think I achieved that. :contentReference[oaicite:31]{index=31}  
- **Pentagon UI**: Visually intuitive for connecting wave shapes and triggering interior effect toggles. I chose a pentagon because my initial idea grew too large and I needed to shave down for time. A pentagon allows for some "glyph like" stylization. :contentReference[oaicite:32]{index=32}  

---

