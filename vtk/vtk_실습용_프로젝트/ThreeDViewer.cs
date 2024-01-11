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
                    vtkCellPicker picker=new vtkCellPicker();

                    //마우스가 찍힌 화면 스크린 픽셀 좌표계 얻어내기.
                    var pos = codi.Window.interaction.inst.GetInteractor().GetEventPosition();
                    int xPos=pos.ElementAt(0);
                    int yPos=pos.ElementAt(1);

                    //렌더러에서 피킹 시도
                    picker.Pick(xPos, yPos, 0, codi.GetVtkRenderer());

                    //피킹된 액터 (아무것도 피킹되지 않았다면 null 일 수 있음)
                    var pickedActor=picker.GetActor(); 

                    if(-1==picker.GetCellId()||pickedActor==null)
                    {
                        //액터 픽 실패
                        //if(pickedActor == null)
                        //    Console.WriteLine("null");
                        //else
                        //    Console.WriteLine(picker.GetCellId());
                        return false;
                    }

                    //vtkPolyData polyData = pickedActor.GetMapper().GetInputAsDataSet() as vtkPolyData;
                    //long cellId = picker.GetCellId();
                    //vtkDataArray colors = polyData.GetCellData().GetScalars();

                    //if (colors != null)
                    //{
                    //    double r = colors.GetComponent(cellId, 0);
                    //    double g = colors.GetComponent(cellId, 1);
                    //    double b = colors.GetComponent(cellId, 2);
                    //    Console.WriteLine($"RGB of picked cell: {r}, {g}, {b}");
                    //}

                    Console.WriteLine(pickedActor.GetObjectName());

                    //피킹된 3차원 vtk 좌표계 얻어내기
                    double[] pickedPosition = picker.GetPickPosition();

                    //c# 문자열 포맷
                    //printf 생각하면 됩니다.
                    //{0} {1} 자리에 첫번째, 두번째 인자가 문자열로 해석돼서 들어가요.
                    Console.WriteLine("x:{0}, y:{1}, z:{2}", pickedPosition[0], pickedPosition[1], pickedPosition[2]);

                    //새로운 액터를 만들고, 위에서 얻어낸 pickedPosition 위치로 설정해서 렌더러에 넣어주면,
                    //마우스 클릭할때마다 액터에 다른 액터가 붙게 할 수도 있어요.

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

                  var 렌더러 = viewer.GetVtkRenderer();
                  var 액터들 = 렌더러.GetActors();

                  for (int i = 0; i < 액터들.GetNumberOfItems(); ++i)
                  {
                      var i번째액터 = 액터들.GetItemAsObject(i) as vtkActor;

                      if ("안녕" != i번째액터.GetObjectName())
                          Console.WriteLine(i번째액터.GetObjectName());
                  }


                  return true;
              });

            onchar.Add((sbyte)'b',
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
