using Kitware.VTK;
using System;
using System.Linq;

namespace DensfloReport
{
    static public class ThreeDViewerInfo
    {
        static public readonly ThreeDViewer[] viewerList = new[] { new ThreeDViewer() };
        static public ThreeDViewer[] ViewerList { get { return viewerList; } }
        static public ThreeDViewer Viewer { get { return ViewerList[0]; } }

        static public void Render()
        {
            foreach (var codi in ViewerList)
            {
                codi.Render();
            }
        }

        static public void InitCustomInteraction(ThreeDViewer viewer)
        {
            setDoubleClickAddMarker(viewer);
            setLeftClickSelectMarker(viewer);
            setRightClickOpenMarkerMenu(viewer);
            Debug_SetChar(viewer);
        }

        static void setRightClickOpenMarkerMenu(ThreeDViewer viewer)
        {
            viewer.Window.interaction.currentInteractionFunctionSet.OnRightButtonDown
              = () =>
              {
                  return false;
              };
        }

        static void setLeftClickSelectMarker(ThreeDViewer codi)
        {
            // 더블클릭 함수 설정.
            codi.Window.interaction.currentInteractionFunctionSet.OnLeftButtonDown
                = () =>
                {
                    return false;
                };
        }

        static void setDoubleClickAddMarker(ThreeDViewer codi)
        {
            // 더블클릭 함수 설정.
            codi.Window.interaction.currentInteractionFunctionSet.OnLeftDoubleClick
                = () =>
                {

                };
        }

        static void Debug_SetChar(ThreeDViewer viewer)
        {
            ref var onchar = ref viewer.Interactor.currentInteractionFunctionSet.OnChar;
            onchar.Clear();

            for (int _i = 1; _i <= 8; _i++)
            {
                int idx = _i + 1;
                onchar.Add((sbyte)idx.ToString().First(),
                 () =>
                 {
                     return true;
                 });
            }

            onchar.Add((sbyte)'a',
              () =>
              {

                  vtkPoints points = vtkPoints.New();
                  double[,] p = new double[,] {
            { 1.0,  1.0, 1.0 },
            {-1.0,  1.0, 1.0 },
            {-1.0, -1.0, 1.0 },
            { 1.0, -1.0, 1.0 },
            { 0.0,  0.0, 0.0 }};

                  for (int i = 0; i < 5; i++)
                      points.InsertNextPoint(p[i, 0], p[i, 1], p[i, 2]);

                  vtkPyramid pyramid = vtkPyramid.New();
                  for (int i = 0; i < 5; i++)
                      pyramid.GetPointIds().SetId(i, i);

                  vtkCellArray cells = vtkCellArray.New();
                  cells.InsertNextCell(pyramid);

                  vtkUnstructuredGrid ug = vtkUnstructuredGrid.New();
                  ug.SetPoints(points);
                  ug.InsertNextCell(pyramid.GetCellType(), pyramid.GetPointIds());

                  //Create an actor and mapper
                  vtkDataSetMapper mapper = vtkDataSetMapper.New();
                  mapper.SetInputData(ug);

                  vtkActor actor = vtkActor.New();
                  actor.RotateX(105.0);
                  actor.RotateZ(-36.0);
                  actor.SetMapper(mapper);

                  actor.GetProperty().SetColor(1, 0, 0);

                  viewer.GetVtkRenderer(0).AddActor(actor);


                  var testactor = viewer.GetVtkRenderer().GetActors().GetItemAsObject(0) as vtkActor;
                  testactor.VisibilityOff();

                  return true;
              });

            onchar.Add((sbyte)'s',
              () =>
              {

                  // Create two arrows.  
                  vtkArrowSource arrowSource01 = vtkArrowSource.New();
                  vtkArrowSource arrowSource02 = vtkArrowSource.New();
                  arrowSource02.SetShaftResolution(24);   // default = 6
                  arrowSource02.SetTipResolution(36);     // default = 6

                  // Visualize
                  vtkPolyDataMapper mapper01 = vtkPolyDataMapper.New();

                  mapper01.SetInputConnection(arrowSource01.GetOutputPort());

                  vtkActor actor01 = vtkActor.New();

                  actor01.SetMapper(mapper01);

                  actor01.SetPosition(0.0, 0.25, 0.0);

                  actor01.GetProperty().SetColor(1, 0, 1);

                  viewer.GetVtkRenderer(0).AddActor(actor01);

                  var testactor = viewer.GetVtkRenderer().GetActors().GetItemAsObject(0) as vtkActor;
                  testactor.VisibilityOff();

                  return true;
              });

            onchar.Add((sbyte)'w',
            () =>
            {

                return true;
            });

            onchar.Add((sbyte)27, // ESC 키의 ASCII 값은 27입니다.
            () =>
            {
                 // 종료 코드 추가
            Environment.Exit(0);
             return true;
            });
        }
    }

    public class ThreeDViewer
    {
        public readonly ThreeD_RenderWindow Window = new ThreeD_RenderWindow();
        RenderWindowControl RenderWindowControl { get { return Window.renWinCntl; } }
        public InteractorStyle Interactor { get { return Window.interaction; } }
        public DateTime LastFocusedTime { get { return Interactor.focusedTime; } }

        public void Initalize(RenderWindowControl viewerRwctl)
        {
            // 렌더컨트롤로부터 렌더러 세팅
            Window.Initalize(viewerRwctl);

            // 렌더링 시작
            Window.renWinCntl.RenderWindow.Render();
        }

        #region ---------- 렌더링 ---------------

        public vtkRendererCollection GetVtkRendererCollection()
        {
            return RenderWindowControl.RenderWindow.GetRenderers();
        }
        public vtkRenderer GetVtkRenderer(int idx = 0)
        {
            return Window.GetRenderer(idx);
        }
        public void ResetCamera()
        {
            GetVtkRenderer().ResetCamera();
            Render();
        }
        public void Render()
        {
            RenderWindowControl.RenderWindow.GetInteractor().Render();
        }

        #endregion

    }

}
