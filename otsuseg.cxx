/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#include "SegMainWindow.h"

#include <mitkProperties.h>
#include <mitkPointSet.h>

#include <itkCurvatureFlowImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <mitkImageAccessByItk.h>
#include <mitkImageCast.h>
#include <mitkLevelWindowProperty.h>

template < typename TPixel, unsigned int VImageDimension >
void OtsuSegmentation(itk::Image<TPixel, VImageDimension>* itkImage, SegMainWindow* segWindow)
{
	typedef itk::Image< TPixel, VImageDimension > ImageType;
	typedef itk::OtsuThresholdImageFilter <ImageType, ImageType> FilterType;
	FilterType::Pointer otsuFilter
		= FilterType::New();
	otsuFilter->SetInput(itkImage);
	otsuFilter->Update();

	mitk::Image::Pointer mitkImage = mitk::Image::New();
	mitk::CastToMitkImage(otsuFilter->GetOutput(), mitkImage);

	if (segWindow->m_ResultNode.IsNull())
	{
		segWindow->m_ResultNode = mitk::DataNode::New();
		segWindow->m_DataStorage->Add(segWindow->m_ResultNode);
	}
	segWindow->m_ResultNode->SetData(mitkImage);
	// set some additional properties
	segWindow->m_ResultNode->SetProperty("name", mitk::StringProperty::New("segmentation"));
	segWindow->m_ResultNode->SetProperty("binary", mitk::BoolProperty::New(true));
	segWindow->m_ResultNode->SetProperty("color", mitk::ColorProperty::New(1.0, 0.0, 0.0));
	segWindow->m_ResultNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
	segWindow->m_ResultNode->SetProperty("layer", mitk::IntProperty::New(1));
	mitk::LevelWindowProperty::Pointer levWinProp = mitk::LevelWindowProperty::New();
	mitk::LevelWindow levelwindow;
	levelwindow.SetAuto(mitkImage);
	levWinProp->SetLevelWindow(levelwindow);
	segWindow->m_ResultNode->SetProperty("levelwindow", levWinProp);

	segWindow->m_ResultImage = static_cast<mitk::Image*>(segWindow->m_ResultNode->GetData());
}

/**
\example Step6RegionGrowing.txx
*/
