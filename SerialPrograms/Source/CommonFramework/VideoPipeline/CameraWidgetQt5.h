/*  Video Widget (Qt5)
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_VideoPipeline_Qt5VideoWidget_H
#define PokemonAutomation_VideoPipeline_Qt5VideoWidget_H

#include <condition_variable>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVideoProbe>
#include "Common/Cpp/SpinLock.h"
#include "CommonFramework/Logging/LoggerQt.h"
#include "CameraInfo.h"
#include "VideoWidget.h"

namespace PokemonAutomation{
namespace CameraQt5{


std::vector<CameraInfo> qt5_get_all_cameras();
QString qt5_get_camera_name(const CameraInfo& info);


class Qt5VideoWidget : public VideoWidget{
public:
    Qt5VideoWidget(
        QWidget* parent,
        LoggerQt& logger,
        const CameraInfo& info, const QSize& desired_resolution
    );
    virtual ~Qt5VideoWidget();
    virtual QSize resolution() const override;
    virtual std::vector<QSize> resolutions() const override;
    virtual void set_resolution(const QSize& size) override;

    //  Cannot call from UI thread or it will deadlock.
    virtual QImage snapshot(WallClock* timestamp) override;

    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QImage snapshot_probe(WallClock* timestamp);
    QImage snapshot_image(WallClock* timestamp);

private:
    enum class CaptureStatus{
        PENDING,
        COMPLETED,
        CANCELED,
    };
    struct PendingCapture{
        CaptureStatus status = CaptureStatus::PENDING;
        QImage image;
        std::condition_variable cv;
    };

    LoggerQt& m_logger;
    QCamera* m_camera = nullptr;
    QCameraViewfinder* m_camera_view = nullptr;

    size_t m_max_frame_rate;
    std::chrono::milliseconds m_frame_period;
    std::vector<QSize> m_resolutions;

    mutable std::mutex m_lock;
    QSize m_resolution;

    QCameraImageCapture* m_capture = nullptr;
    std::map<int, PendingCapture> m_pending_captures;

    SpinLock m_frame_lock;
    std::atomic<WallClock> m_last_snapshot;

//    SpinLock m_capture_lock;
    QVideoProbe* m_probe = nullptr;
    std::atomic<uint64_t> m_seqnum_frame;
//    uint64_t m_seqnum_frame = 0;
//    QVideoFrame m_last_frame;
    uint64_t m_seqnum_image = 0;
    QImage m_last_image;
};



}
}
#endif
