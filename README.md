# Frequency Shifter

Basic frequency shifter implementation using [Signalsmith's IIR Hilbert Filter](https://github.com/Signalsmith-Audio/hilbert-iir) and [JUCE](https://github.com/juce-framework/JUCE).

## TODO

- ~~Further filter the signal to mitigate aliasing.~~ ✅
- Try reducing latency by reducing the order of the Hilbert IIR filter.
  - Currently has ~35 samples of latency @ 44.1kHz.
- SIMD implementation.

## License

MIT License.