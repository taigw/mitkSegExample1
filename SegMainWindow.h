
#ifndef __SegMainWindow_H__
#define __SegMainWindow_H__

#include <QMainWindow>
#include <qwidget.h>
#include <qlabel.h>
#include <QmitkStdMultiWidget.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>
#include <mitkDataNode.h>
#include <itkImage.h>

namespace Ui {
	class SegMainWindow;
}

class SegMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit SegMainWindow(QWidget *parent = 0);
	~SegMainWindow();
public slots:
	void sltOpenFile();
	void sltThresholding();
protected:
	template < typename TPixel, unsigned int VImageDimension >
	friend void OtsuSegmentation(itk::Image<TPixel, VImageDimension>* itkImage, SegMainWindow* segWindow);
private:

	void CreateActions();
	void CreateMenus();

	QAction * openFileAction;
	QAction * thresholdAction;
	QAction * exitAction;

	QLabel *label;
	QmitkStdMultiWidget *multiViewWgt;

	mitk::StandaloneDataStorage::Pointer m_DataStorage;
	mitk::Image::Pointer m_FirstImage;
	mitk::Image::Pointer m_ResultImage;
	mitk::DataNode::Pointer m_ResultNode;
};
#endif