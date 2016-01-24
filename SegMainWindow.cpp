#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qfiledialog.h>

#include <mitkIOUtil.h>
#include <QmitkDataStorageComboBox.h>
#include <QmitkDataStorageTreeModel.h>
#include "mitkProperties.h"
#include "mitkImageAccessByItk.h"
#include "mitkRenderingManager.h"
#include <itkOtsuThresholdImageFilter.h>

#include <vtkMarchingCubes.h>
#include "SegMainWindow.h"
SegMainWindow::SegMainWindow(QWidget *parent) :
QMainWindow(parent)
{
	label = new QLabel("Seg Tool");
	QHBoxLayout * layout = new QHBoxLayout(this);;
	multiViewWgt = new QmitkStdMultiWidget;


	QWidget *centerWgt = new QWidget;
	layout->addWidget(multiViewWgt);

	centerWgt->setLayout(layout);
	this->setCentralWidget(centerWgt);
	//this->setLayout(layout);
	this->setMinimumSize(800,600);

	m_DataStorage = mitk::StandaloneDataStorage::New();
	multiViewWgt->SetDataStorage(m_DataStorage);
	// Initialize bottom-right view as 3D view
	multiViewWgt->GetRenderWindow4()->GetRenderer()->SetMapperID(
		mitk::BaseRenderer::Standard3D);

	// Enable standard handler for levelwindow-slider
	multiViewWgt->EnableStandardLevelWindow();
	multiViewWgt->SetWidgetPlanesVisibility(true);
	multiViewWgt->DisableDepartmentLogo();
	
	CreateActions();
	CreateMenus();
	m_FirstImage = nullptr;
	m_ResultImage = nullptr;
	m_ResultNode = nullptr;
}

SegMainWindow::~SegMainWindow()
{

}

void SegMainWindow::CreateActions()
{
	openFileAction = new QAction(tr("&Open File"), this);
	exitAction = new QAction(tr("&Exit"), this);

	connect(openFileAction, SIGNAL(triggered()), this, SLOT(sltOpenFile()));

	thresholdAction = new QAction(tr("&Threshold Segmentation"), this);
	connect(thresholdAction, SIGNAL(triggered()), this, SLOT(sltThresholding()));
}

void SegMainWindow::CreateMenus()
{
	QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(exitAction);

	QMenu * operationMenu = menuBar()->addMenu(tr("&Operation"));
	operationMenu->addAction(thresholdAction);
}

void SegMainWindow::sltOpenFile()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();
	std::string fileName = fileNames[0].toStdString();

	mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes;
	for (int i = 0; i < fileNames.length(); i++)
	{
		fileName  = fileNames[i].toStdString();
		dataNodes=mitk::IOUtil::Load(fileName, *m_DataStorage);
	}
	if (dataNodes->empty())
	{
		fprintf(stderr, "Could not open file %s \n\n", fileName);
		exit(2);
	}

	mitk::Image::Pointer image = dynamic_cast<mitk::Image*> (dataNodes->at(0)->GetData());
	if ((m_FirstImage.IsNull()) && (image.IsNotNull()))
		m_FirstImage = image;

	multiViewWgt->ResetCrosshair();
	multiViewWgt->AddDisplayPlaneSubTree();
	multiViewWgt->AddPlanesToDataStorage();
}

void SegMainWindow::sltThresholding()
{
	AccessByItk_1(m_FirstImage, OtsuSegmentation, this);
	if (m_ResultImage.IsNotNull())
	{
		m_ResultNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));

		vtkMarchingCubes* surfaceCreator = vtkMarchingCubes::New();
		surfaceCreator->SetInputData(m_ResultImage->GetVtkImageData());
		surfaceCreator->SetValue(0, 1);
		surfaceCreator->Update();

		mitk::Surface::Pointer surface = mitk::Surface::New();
		surface->SetVtkPolyData(surfaceCreator->GetOutput()); //VTK6_TODO

		mitk::DataNode::Pointer surfaceNode = mitk::DataNode::New();
		surfaceNode->SetData(surface);

		m_DataStorage->Add(surfaceNode);

		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		surfaceCreator->Delete();
	}
	//mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}