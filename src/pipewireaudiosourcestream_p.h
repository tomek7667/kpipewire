/*
    SPDX-FileCopyrightText: 2026 Khudoberdi <xudoyberdi0410@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <QObject>
#include <chrono>

#include <pipewire/pipewire.h>

#include <kpipewire_export.h>

struct PipeWireAudioSourceStreamPrivate;

struct PipeWireAudioFrame {
    /// Interleaved F32 samples, only valid for the duration of the framesReceived() emission
    const float *data;
    quint32 sampleCount; ///< Number of samples per channel
    quint32 channels;
    quint32 rate;
    std::chrono::nanoseconds presentationTimestamp; ///< CLOCK_MONOTONIC
};

class KPIPEWIRE_EXPORT PipeWireAudioSourceStream : public QObject
{
    Q_OBJECT
public:
    enum class Source {
        OutputMonitor, ///< Capture what is being played on an output device
        Input, ///< Capture an input device, e.g. a microphone
    };

    /**
     * @p targetNodeName is the node.name of the device to capture. If it is
     * empty, the default device of the @p source kind is captured instead,
     * following it when the default changes.
     */
    explicit PipeWireAudioSourceStream(Source source, const QString &targetNodeName = {}, QObject *parent = nullptr);
    ~PipeWireAudioSourceStream();

    bool createStream();
    void setActive(bool active);
    pw_stream_state state() const;
    QString error() const;

    void handleFrame(struct pw_buffer *buffer);
    void process();

Q_SIGNALS:
    void framesReceived(const PipeWireAudioFrame &frame);
    void stateChanged(pw_stream_state state, pw_stream_state oldState);
    void stopStreaming();

private:
    static void onStreamParamChanged(void *data, uint32_t id, const struct spa_pod *format);
    static void onStreamStateChanged(void *data, pw_stream_state old, pw_stream_state state, const char *error_message);
    static void onDestroy(void *data);

    void coreFailed(const QString &errorMessage);
    QScopedPointer<PipeWireAudioSourceStreamPrivate> d;
};
