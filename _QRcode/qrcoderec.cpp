#include "qrcoderec.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
using namespace cv;

QRcodeRec::QRcodeRec()
{
    LoadModel();
}

QRcodeRec::~QRcodeRec()
{

}

void QRcodeRec::LoadModel()
{
    QString filepath = QString("../config/config_qr.ini");
    QFileInfo file = QFileInfo(filepath);
    if(!file.exists())
    {
        MDEBUG << "The QR configuration file does not exist" ;
    }

    QSettings settings(filepath,QSettings::IniFormat);
    settings.beginGroup("MODELPATH");
    std::string detect_prototxt = settings.value("Detect_Prototext").toString().toStdString();
    std::string detect_caffe_model = settings.value("Detect_Caffe_Model").toString().toStdString();
    std::string sr_prototxt = settings.value("Sr_Prototext").toString().toStdString();
    std::string sr_caffe_model = settings.value("Sr_Caffe_Model").toString().toStdString();
    settings.endGroup();
    LoadModel(detect_caffe_model,detect_prototxt,sr_caffe_model,sr_prototxt);
}

void QRcodeRec::LoadModel(std::string det_model, std::string det_prototxt, std::string sr_model, std::string sr_prototxt)
{
    try{
        _detector = cv::makePtr<cv::wechat_qrcode::WeChatQRCode>(det_prototxt,det_model,sr_prototxt,sr_model);
    }
    catch(const std::exception& e)
    {
        MDEBUG << "Failed to load two-dimensional code recognition model!!!";
    }
}

/**
 * @brief QRcodeRec::runModel 执行扫二维码功能
 * @param img The input image
 * @param rect The offset of the roi region compared to the original image
 * @return 返回二维码识别结果及坐标
 */
QVector<QRPoint> QRcodeRec::runModel(cv::Mat img,cv::Rect rect)
{
    std::vector<cv::Mat> vPoints;
    std::vector<cv::String> strDecoded;
    QVector<QRPoint> vec_point;
    QRPoint qr_point;
    strDecoded = _detector->detectAndDecode(img, vPoints);

    for (int i = 0; i < strDecoded.size(); i++)
    {
     Point pt1 = Point((int)vPoints[i].at<float>(0, 0) + rect.x, (int)vPoints[i].at<float>(0, 1) + rect.y);
     qr_point.upleft = pt1;
     Point pt2 = Point((int)vPoints[i].at<float>(1, 0) + rect.x, (int)vPoints[i].at<float>(1, 1) + rect.y);
     qr_point.upright = pt2;
     Point pt3 = Point((int)vPoints[i].at<float>(2, 0) + rect.x, (int)vPoints[i].at<float>(2, 1) + rect.y);
     qr_point.bottomright = pt3;
     Point pt4 = Point((int)vPoints[i].at<float>(3, 0) + rect.x, (int)vPoints[i].at<float>(3, 1) + rect.y);
     qr_point.bottomleft = pt4;
     QString res = QString::fromStdString(strDecoded[i]);
     qr_point.res = res;
     putText(img, strDecoded[i], pt1, 0, 0.5, Scalar(255, 0, 0), 2);
     vec_point.push_back(qr_point);
    }
    return vec_point;
}

/**
 * @brief QRcodeRec::displayPosition 矫正二维码
 * @param src_image The input image
 * @param qrcode res->point
 * @param roi_rect
 */
void QRcodeRec::correctQrMat(Mat img)
{
    Mat srcImg = img.clone();
    Mat contrastImage = Mat::zeros(srcImg.size(),srcImg.type());
}

/**
 * @brief QRcodeRec::displayPosition 在原图像中显示二维码位置
 * @param src_image The input image
 * @param qrcode res->point
 * @param roi_rect
 */
void QRcodeRec::displayPosition(Mat &src, QRPoint qr_point, Rect rect)
{
    line(src,qr_point.upleft,qr_point.upright,Scalar(0,255,0),2);
    line(src,qr_point.upright,qr_point.bottomright,Scalar(0,255,0),2);
    line(src,qr_point.bottomright,qr_point.bottomleft,Scalar(0,255,0),2);
    line(src,qr_point.bottomleft,qr_point.upleft,Scalar(0,255,0),2);
}

/**
 * @brief QRcodeRec::removeQRRegion 去除图像中二维码区域，避免ocr误识别
 * @param roi_image The input image
 * @param qrcode res->point
 * @param roi_rect
 */
void QRcodeRec::removeQRRegion(cv::Mat& src,QRPoint qr_point,cv::Rect rect)
{
    // 构造矩形区域
   cv::Point pt1 = cv::Point((int)qr_point.upleft.x - rect.x,(int)qr_point.upleft.y - rect.y);
   cv::Point pt2 = cv::Point((int)qr_point.upright.x - rect.x,(int)qr_point.upright.y - rect.y);
   cv::Point pt3 = cv::Point((int)qr_point.bottomright.x - rect.x,(int)qr_point.bottomright.y - rect.y);
   cv::Point pt4 = cv::Point((int)qr_point.bottomleft.x - rect.x,(int)qr_point.bottomleft.y - rect.y);
   cv::Point pts[] = { pt1,pt2,pt3,pt4 };
   cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
   cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255)); // 填充多边形区域

   // 将矩形区域置为0
   src.setTo(cv::Scalar(0), mask);
}

