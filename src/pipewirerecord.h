/*
    SPDX-FileCopyrightText: 2022 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <QObject>
#include <QStringList>
#include <qqmlintegration.h>

#include "pipewirebaseencodedstream.h"
#include <kpipewire_export.h>

struct PipeWireRecordPrivate;

class KPIPEWIRE_EXPORT PipeWireRecord : public PipeWireBaseEncodedStream
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(QString extension READ extension NOTIFY encoderChanged)
    Q_PROPERTY(bool recordSystemAudio READ recordSystemAudio WRITE setRecordSystemAudio NOTIFY recordSystemAudioChanged)
    Q_PROPERTY(bool recordMicrophone READ recordMicrophone WRITE setRecordMicrophone NOTIFY recordMicrophoneChanged)
    Q_PROPERTY(QStringList recordedAudioSinks READ recordedAudioSinks WRITE setRecordedAudioSinks NOTIFY recordedAudioSinksChanged)
    Q_PROPERTY(QStringList recordedAudioSources READ recordedAudioSources WRITE setRecordedAudioSources NOTIFY recordedAudioSourcesChanged)
public:
    PipeWireRecord(QObject *parent = nullptr);
    ~PipeWireRecord() override;

    QString output() const;
    void setOutput(const QString &output);
    QString extension() const;

    /// Whether to also record what is being played on the default audio output
    bool recordSystemAudio() const;
    void setRecordSystemAudio(bool recordSystemAudio);
    /// Whether to also record the default audio input, e.g. a microphone
    bool recordMicrophone() const;
    void setRecordMicrophone(bool recordMicrophone);

    /**
     * Audio output devices whose playback is also recorded, identified by the
     * node.name of their PipeWire Audio/Sink node.
     *
     * Unlike recordSystemAudio, these stay on the given device even when the
     * default output changes. A device that is missing is recorded as silence
     * until it appears, rather than falling back to another device.
     * All recorded audio is mixed into a single track.
     */
    QStringList recordedAudioSinks() const;
    void setRecordedAudioSinks(const QStringList &nodeNames);

    /**
     * Audio input devices that are also recorded, identified by the node.name
     * of their PipeWire Audio/Source node.
     *
     * Unlike recordMicrophone, these stay on the given device even when the
     * default input changes. A device that is missing is recorded as silence
     * until it appears, rather than falling back to another device.
     * All recorded audio is mixed into a single track.
     */
    QStringList recordedAudioSources() const;
    void setRecordedAudioSources(const QStringList &nodeNames);

    // Only for compatibility with 5.27
    KPIPEWIRE_DEPRECATED QString currentExtension() const
    {
        return extension();
    }

Q_SIGNALS:
    void outputChanged(const QString &output);
    void recordSystemAudioChanged(bool recordSystemAudio);
    void recordMicrophoneChanged(bool recordMicrophone);
    void recordedAudioSinksChanged(const QStringList &nodeNames);
    void recordedAudioSourcesChanged(const QStringList &nodeNames);

private:
    std::unique_ptr<PipeWireProduce> makeProduce() override;

    QScopedPointer<PipeWireRecordPrivate> d;
};
