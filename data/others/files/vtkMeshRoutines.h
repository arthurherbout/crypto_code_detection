/****************************************************************************
** Copyright (c) 2017 Adrian Schneider, AOT AG
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
**
*****************************************************************************/

#ifndef _vtkMeshRoutines_H_
#define _vtkMeshRoutines_H_

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkVector.h>
#include <vtkCallbackCommand.h>
#include <string>
#include <vector>

class VTKMeshRoutines
{

public:

    VTKMeshRoutines();
    ~VTKMeshRoutines();

    /**
     * Sets the progress callback function.
     * @param progressCallback A progress callback.
     */
    void SetProgressCallback( vtkSmartPointer<vtkCallbackCommand> progressCallback );

    /**
     * Moves the mesh to center of the coordinate system. In particular,
     * the center of mass is computed and the mesh is translated accordingly.
     * @param mesh The input mesh. Mesh will be modified afterwards.
     * @return The vector by which the mesh was moved.
     */
    vtkVector3d moveMeshToCOSCenter( vtkSmartPointer<vtkPolyData> mesh );

    /**
     * Reduces the size / details of a 3D mesh.
     * @param mesh The input mesh. Mesh will be modified afterwards.
     * @param reduction Reduction factor. 0.1 is little reduction. 0.9 is strong reduction.
     */
    void meshReduction(vtkSmartPointer<vtkPolyData> mesh, double reduction );

    /**
     * Labels connected regions and removes regions below a certain size.
     * @param mesh The input mesh. Mesh will be modified afterwards.
     * @param ratio Value between 0.0 - 1.0. Value of 0.2 indicates that only
     *              object with a minimum number of 20% vertices relative to the
     *              number of vertices in the largest objects are extracted.
     */
    void removeSmallObjects(vtkSmartPointer<vtkPolyData> mesh, double ratio );

    /**
     * Smooths the mesh surface.
     * @param mesh The input mesh. Mesh will be modified afterwards.
     * @param nbrOfSmoothingIterations Number of smoothing iterations.
     */
    //Todo: Understand FeatureAngle and RelaxationFactor. Then add it as argument.
    void smoothMesh( vtkSmartPointer<vtkPolyData> mesh, unsigned int nbrOfSmoothingIterations );

private:

    vtkSmartPointer<vtkCallbackCommand> m_progressCallback;
};

#endif // _vtkMeshRoutines_H_
