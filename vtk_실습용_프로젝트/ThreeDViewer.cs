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
                  Console.WriteLine("a");
                  return true;
              });

            onchar.Add((sbyte)'s',
              () =>
              {
                  return true;
              });

            onchar.Add((sbyte)'w',
            () =>
            {
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
