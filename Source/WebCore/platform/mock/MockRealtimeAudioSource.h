/*
 * Copyright (C) 2015-2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Ericsson nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(MEDIA_STREAM)

#include "ImageBuffer.h"
#include "MockMediaDevice.h"
#include "RealtimeMediaSourceFactory.h"
#include <wtf/RunLoop.h>
#include <wtf/WorkQueue.h>

namespace WebCore {

class MockRealtimeAudioSource : public RealtimeMediaSource {
public:
    static CaptureSourceOrError create(String&& deviceID, String&& name, String&& hashSalt, const MediaConstraints*);
    virtual ~MockRealtimeAudioSource();

protected:
    MockRealtimeAudioSource(String&& deviceID, String&& name, String&& hashSalt);

    virtual void render(Seconds) = 0;
    void settingsDidChange(OptionSet<RealtimeMediaSourceSettings::Flag>) override;

    static Seconds renderInterval() { return 60_ms; }

private:
    const RealtimeMediaSourceCapabilities& capabilities() final;
    const RealtimeMediaSourceSettings& settings() final;

    void startProducingData() final;
    void stopProducingData() final;

    bool isCaptureSource() const final { return true; }
    CaptureDevice::DeviceType deviceType() const final { return CaptureDevice::DeviceType::Microphone; }

    void delaySamples(Seconds) final;

    void tick();

protected:
    Ref<WorkQueue> m_workQueue;

private:
    Optional<RealtimeMediaSourceCapabilities> m_capabilities;
    Optional<RealtimeMediaSourceSettings> m_currentSettings;
    RealtimeMediaSourceSupportedConstraints m_supportedConstraints;

    RunLoop::Timer<MockRealtimeAudioSource> m_timer;
    MonotonicTime m_startTime { MonotonicTime::nan() };
    MonotonicTime m_lastRenderTime { MonotonicTime::nan() };
    Seconds m_elapsedTime { 0_s };
    MonotonicTime m_delayUntil;
    MockMediaDevice m_device;
};

} // namespace WebCore

#endif // ENABLE(MEDIA_STREAM)
