#ifndef QRCODEREC_H
#define QRCODEREC_H

#include <QObject>
#include <opencv2/wechat_qrcode.hpp>
#include <opencv2/opencv.hpp>
#include <QThread>
#ifndef MDEBUG
#define MDEBUG      qDebug() << QThread::currentThreadId() << "\t" << __LINE__ << "\t" << __FUNCTION__
#endif

struct QRPoint{
    cv::Point upleft;
    cv::Point upright;
    cv::Point bottomright;
    cv::Point bottomleft;
    QString res;
};

namespace Ui {
class QRcodeRec;
}

class QRcodeRec : public QObject
{
    Q_OBJECT
public:
     QRcodeRec();
    ~QRcodeRec();

     void LoadModel();
     void LoadModel(std::string det_model,std::string det_prototxt,std::string sr_model,std::string sr_prototxt);
     QVector<QRPoint> runModel(cv::Mat img,cv::Rect rect);
     void correctQrMat(cv::Mat img);
     void static displayPosition(cv::Mat& src,QRPoint qr_point,cv::Rect rect);
     void static removeQRRegion(cv::Mat& src,QRPoint qr_point,cv::Rect rect);

private:
    cv::Ptr<cv::wechat_qrcode::WeChatQRCode> _detector;
};

#endif // QRCODEREC_H
