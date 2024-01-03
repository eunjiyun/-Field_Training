using Kitware.VTK;
using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace DensfloReport
{
    public class InteractionOverrideFucntionSet
    {
        public Func<bool> OnLeftButtonDown = () => { return false; };
        public Func<bool> OnLeftButtonUp = () => { return false; };
        public Action OnLeftDoubleClick = () => { };
        public Func<bool> OnRightButtonDown = () => { return false; };
        public Func<bool> OnRightButtonUp = () => { return false; };
        public Action OnRightDoubleClick = () => { };
        public Func<bool> OnMiddleButtonDown = () => { return false; };
        public Func<bool> OnMiddleButtonUp = () => { return false; };
        public Action OnMiddleDoubleClick = () => { };
        public Func<bool> OnMouseMove = () => { return false; };

        public Func<object, EventArgs, bool> CheckIsFirstRenderPass = (s, e) => { return false; };
        public Action<object, EventArgs> OnFirstRender = (s, e) => { };
        public Action<object, EventArgs> RenderSecondRenderPass = (s, e) => { };
        public Action<object, EventArgs> OnSecondRender = (s, e) => { };

        public Dictionary<sbyte, Func<bool>> OnChar = new Dictionary<sbyte, Func<bool>>();
        //
    };

    public class InteractorStyle
    {
        public vtkInteractorStyleTrackballCamera inst;
        public InteractionOverrideFucntionSet currentInteractionFunctionSet;
        public bool disable;
        public DateTime focusedTime = DateTime.MinValue;

        public InteractorStyle()
        {
            inst = vtkInteractorStyleTrackballCamera.New();

            currentInteractionFunctionSet = new InteractionOverrideFucntionSet();

            disable = false;

            inst.CharEvt += OnChar;
            inst.MouseMoveEvt += OnMouseMove;
            inst.LeftButtonPressEvt += OnLeftButtonDown;
            inst.LeftButtonReleaseEvt += OnLeftButtonUp;
            inst.RightButtonPressEvt += OnRightButtonDown;
            inst.RightButtonReleaseEvt += OnRightButtonUp;
            inst.MiddleButtonPressEvt += OnMiddleButtonDown;
            inst.MiddleButtonReleaseEvt += OnMiddleButtonUp;
            inst.MouseWheelBackwardEvt += OnMouseWheelBackward;
            inst.MouseWheelForwardEvt += OnMouseWheelForward;
        }

        // after set interactor
        public void InitRenderEvt()
        {
            if (null != inst.GetInteractor())
                inst.GetInteractor().RenderEvt += OnRender;
        }

        public void OnRender(object s, EventArgs e)
        {

            if (!currentInteractionFunctionSet.CheckIsFirstRenderPass(s, e))
            {
                // 두 번째 렌더패스 이후
                currentInteractionFunctionSet.OnSecondRender(s, e);
                return;
            }

            // 첫 번째 렌더패스 이후
            currentInteractionFunctionSet.OnFirstRender(s, e);

            // 두 번째 렌더패스 실행
            currentInteractionFunctionSet.RenderSecondRenderPass(s, e);
        }

        public void OnMouseMove(object sender, EventArgs e)
        {
            if (disable) return;
            if (currentInteractionFunctionSet.OnMouseMove())
            {
                return;
            }
            inst.OnMouseMove();
        }

        public void OnLeftButtonDown(object sender, EventArgs e)
        {
            if (disable) return;
            focusedTime = DateTime.Now;
            if (_doubleClickCheck())
            {
                currentInteractionFunctionSet.OnLeftDoubleClick();
                return;
            }
            if (currentInteractionFunctionSet.OnLeftButtonDown())
            {
                return;
            }
            inst.OnLeftButtonDown();
        }

        public void OnLeftButtonUp(object sender, EventArgs e)
        {
            if (disable) return;

            if (currentInteractionFunctionSet.OnLeftButtonUp())
            {
                return;
            }
            inst.OnLeftButtonUp();
        }

        public void OnRightButtonDown(object sender, EventArgs e)
        {
            if (disable) return;
            focusedTime = DateTime.Now;
            if (_doubleClickCheck())
            {
                currentInteractionFunctionSet.OnRightDoubleClick();
                return;
            }
            if (currentInteractionFunctionSet.OnRightButtonDown())
            {
                return;
            }
            inst.OnRightButtonDown();
        }

        public void OnRightButtonUp(object sender, EventArgs e)
        {
            if (disable) return;
            if (currentInteractionFunctionSet.OnRightButtonUp())
            {
                return;
            }
            inst.OnLeftButtonUp();
        }

        public void OnMiddleButtonDown(object sender, EventArgs e)
        {
            if (disable) return;
            focusedTime = DateTime.Now;
            if (_doubleClickCheck())
            {
                currentInteractionFunctionSet.OnMiddleDoubleClick();
                return;
            }
            if (currentInteractionFunctionSet.OnMiddleButtonDown())
            {
                return;
            }
            inst.OnMiddleButtonDown();
        }

        public void OnMiddleButtonUp(object sender, EventArgs e)
        {
            if (disable) return;
            if (currentInteractionFunctionSet.OnMiddleButtonUp())
            {
                return;
            }
            inst.OnMiddleButtonUp();
        }

        public void OnMouseWheelForward(object sender, EventArgs e)
        {
            if (disable) return;
            focusedTime = DateTime.Now;
            if (_getCameraParallelScale() <= minParallelScale)
                return;

            inst.OnMouseWheelForward();
        }

        public void OnMouseWheelBackward(object sender, EventArgs e)
        {
            if (disable) return;
            focusedTime = DateTime.Now;
            if (maxParallelScale <= _getCameraParallelScale())
                return;

            inst.OnMouseWheelBackward();
        }

        public void OnChar(object sender, EventArgs e)
        {
            if (disable) return;

            // 등록해둔 커스텀 함수가 있다면 실행.
            Func<bool> customFunc;
            if (currentInteractionFunctionSet.OnChar.TryGetValue(inst.GetInteractor().GetKeyCode(), out customFunc))
            {
                // 커스텀 함수가 true 를 리턴하면 다른 인터렉션은 호출 X
                if (customFunc.Invoke())
                {
                    return;
                }
            }

            switch (inst.GetInteractor().GetKeyCode())
            {
                case (sbyte)'r':
                    inst.OnChar();
                    break;
                default:
                    //                    inst.OnChar();
                    break;
            }
        }

        Stopwatch stopwatch = new Stopwatch();
        const long doubleClickThreasholdMs = 200; //ms
        private bool _doubleClickCheck()
        {
            stopwatch.Stop();
            var elapsed = stopwatch.ElapsedMilliseconds;
            stopwatch.Restart();

            if (elapsed == 0 || doubleClickThreasholdMs < elapsed)
                return false;

            stopwatch.Reset();
            inst.StopState();
            return true;
        }

        private double _getCameraParallelScale()
        {
            return inst.GetDefaultRenderer().GetActiveCamera().GetParallelScale();
        }
        private void _setCameraParallelScale(double scale)
        {
            inst.GetDefaultRenderer().GetActiveCamera().SetParallelScale(scale);
        }
        private bool _checkCameraParallelScale()
        {
            if (_getCameraParallelScale() < minParallelScale)
            {
                _setCameraParallelScale(minParallelScale);
                return true;
            }

            if (maxParallelScale < _getCameraParallelScale())
            {
                _setCameraParallelScale(maxParallelScale);
                return true;
            }

            return false;
        }


        const double maxParallelScale = 999999999;
        const double minParallelScale = 0;
    };
}
