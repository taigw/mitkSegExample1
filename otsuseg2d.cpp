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

#include "otsuseg.cxx"

#include <mitkInstantiateAccessFunctions.h>

#define InstantiateAccessFunction_OtsuSegmentation(pixelType, dim) \
	template void OtsuSegmentation(itk::Image<pixelType, dim>*, SegMainWindow*);

/// Instantiate the access function for 3D and all datatypes.
/// Instantiation for 2D is done in Step6RegionGrowing1.
/// Some compilers have memory problems without the explicit
/// instantiation, some even need the separation in 2D/3D.
InstantiateAccessFunctionForFixedDimension(OtsuSegmentation, 2)

/**
\example Step6RegionGrowing2.cpp
*/
