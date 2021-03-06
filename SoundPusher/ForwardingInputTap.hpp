//
//  ForwardingInputTap.hpp
//  SoundPusher
//
//  Created by Daniel Vollmer on 16/12/2015.
//
//

#ifndef ForwardingInputTap_hpp
#define ForwardingInputTap_hpp

#include <vector>

#include <os/log.h>
#include "CoreAudio/CoreAudio.h"

#include "CoreAudioHelper.hpp"

// forward declaration
struct DigitalOutputContext;

/// Takes input from device.stream and forwards it to the outContext as soon as a full packet has been buffered.
struct ForwardingInputTap
{
  /**
   * @param device The device whose data to tap and forward to the outContext.
   * @param stream The input stream on device to forward.
   * @param outContext The output context to forward data to. Must outlive us.
   */
  ForwardingInputTap(AudioObjectID device, AudioObjectID stream, DigitalOutputContext &outContext);

  ~ForwardingInputTap();


  /// Starts IO for the device.
  void Start();
  /// Stops IO for the device.
  void Stop();

  /// The device of this context.
  const AudioObjectID _device;
  /// The input stream on _device of this context.
  const AudioObjectID _stream;
  /// The input format read from _stream.
  const AudioStreamBasicDescription _format;

protected:
  /// IOProc called when the input device has new frames for us.
  static OSStatus DeviceIOProcFunc(AudioObjectID inDevice, const AudioTimeStamp* inNow,
    const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime, AudioBufferList* outOutputData,
    const AudioTimeStamp* inOutputTime, void* inClientData);

  /// The output context to which we send any received data.
  DigitalOutputContext &_outContext;

  /// The logger for the IOProc (which is called from a different (real-time) thread).
  os_log_t _log;
  /// IOProc handle.
  AudioDeviceIOProcID _deviceIOProcID;

  /// Is our IOProc started?
  bool _isRunning;
};

#endif /* ForwardingInputTap_hpp */
