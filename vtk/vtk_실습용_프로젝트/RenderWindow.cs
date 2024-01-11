using Kitware.VTK;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static Kitware.VTK.vtkWidgetRepresentation;


namespace DensfloReport
{
    public class ThreeD_RenderWindow
    {
        public RenderWindowControl renWinCntl;
        public readonly InteractorStyle interaction = new InteractorStyle();
        public readonly vtkOrientationMarkerWidget om = vtkOrientationMarkerWidget.New();

        public void Initalize(RenderWindowControl rwcntl)
        {
            renWinCntl = rwcntl;

            initializeRenderer(GetRenderer());

            createOientMarkerWidget(renWinCntl.RenderWindow.GetInteractor());

            // 커스텀 인터렉션스타일로 설정.
            renWinCntl.RenderWindow.GetInteractor().SetInteractorStyle(interaction.inst);
            interaction.inst.SetInteractor(renWinCntl.RenderWindow.GetInteractor());
            interaction.inst.SetDefaultRenderer(GetRenderer());
            interaction.InitRenderEvt();
        }


        public vtkRenderer GetRenderer(int index = 0)
        {
            return renWinCntl.RenderWindow.GetRenderers().GetItemAsObject(index) as vtkRenderer;
        }

        private void initializeRenderer(vtkRenderer renderer)
        {
            renderer.SetLayer(0);
            renderer.SetBackground(0.9372549019607843, 0.0/*9372549019607843*/, 0.9372549019607843);
        }

        public void ResetWidget()
        {
            om.Off();
            om.On();
        }

        void createOientMarkerWidget(vtkRenderWindowInteractor inst)
        {
            om.SetOrientationMarker(makeAnnotatedCubeActor());
            om.SetInteractor(inst);
            om.SetViewport(0.0, 0.0, 0.1, 0.25);
            om.On();
            om.InteractiveOff();
        }

        vtkPropAssembly makeAnnotatedCubeActor()
        {
            // Annotated Cube setup.
            var annotated_cube = vtkAnnotatedCubeActor.New();
            annotated_cube.SetFaceTextScale(0.366667);

            // Anatomic labeling
            annotated_cube.SetXPlusFaceText("+X");
            annotated_cube.SetXMinusFaceText("-X");
            annotated_cube.SetYPlusFaceText("+Y");
            annotated_cube.SetYMinusFaceText("-Y");
            annotated_cube.SetZPlusFaceText("+Z");
            annotated_cube.SetZMinusFaceText("-Z");

            // Change the vector text colors.
            annotated_cube.GetTextEdgesProperty().SetColor(0, 0, 0);
            annotated_cube.GetTextEdgesProperty().SetLineWidth(1);

            annotated_cube.GetXPlusFaceProperty().SetColor(1, 0.4, 0.4);
            annotated_cube.GetXMinusFaceProperty().SetColor(1, 0.4, 0.4);
            annotated_cube.GetYPlusFaceProperty().SetColor(1, 0.4, 0.4);
            annotated_cube.GetYMinusFaceProperty().SetColor(1, 0.4, 0.4);
            annotated_cube.GetZPlusFaceProperty().SetColor(1, 0.4, 0.4);
            annotated_cube.GetZMinusFaceProperty().SetColor(1, 0.4, 0.4);

            annotated_cube.SetXFaceTextRotation(90);
            annotated_cube.SetYFaceTextRotation(180);
            annotated_cube.SetZFaceTextRotation(-90);
            // Make the annotated cube transparent.
            annotated_cube.GetCubeProperty().SetOpacity(0);

            // Colored faces cube setup.
            var cube_source = vtkCubeSource.New();
            cube_source.Update();

            var cube_mapper = vtkPolyDataMapper.New();
            cube_mapper.SetInputData(cube_source.GetOutput());
            cube_mapper.Update();


            var cube_actor = vtkActor.New();
            cube_actor.GetProperty().SetColor(0.9, 0.9, 0.9);
            cube_actor.GetProperty().SetEdgeColor(0, 0, 0);
            cube_actor.GetProperty().EdgeVisibilityOn();
            cube_actor.SetMapper(cube_mapper);


            // Assemble the colored cube and annotated cube texts into a composite prop.
            var prop_assembly = vtkPropAssembly.New();
            prop_assembly.AddPart(annotated_cube);
            prop_assembly.AddPart(cube_actor);








            //var arrow_source = vtkArrowSource.New();
            //arrow_source.Update();

            //var arrow_mapper = vtkPolyDataMapper.New();
            //arrow_mapper.SetInputData(arrow_source.GetOutput());
            //arrow_mapper.Update();

            //var arrow_actor = vtkActor.New();
            //arrow_actor.GetProperty().SetColor(0.9, 0.9, 0.9);
            //arrow_actor.GetProperty().SetEdgeColor(0, 0, 0);
            //arrow_actor.GetProperty().EdgeVisibilityOn();
            //arrow_actor.SetMapper(arrow_mapper);
            //arrow_actor.RotateZ(-90);

            //var caption_plusX = vtkCaptionActor2D.New();
            //caption_plusX.SetCaption("+Y");
            //double[] position = arrow_actor.GetPosition();
            //caption_plusX.SetAttachmentPoint(position[0], position[1], position[2]);
            //caption_plusX.GetCaptionTextProperty().SetFontSize(12);
            ////arrow_actor.RotateZ(90);

            //// Repeat for -X, +Y, -Y, +Z, -Z

            //var prop_assembly = vtkPropAssembly.New();
            //prop_assembly.AddPart(arrow_actor);
            //prop_assembly.AddPart(caption_plusX);

            return prop_assembly;
        }
    }
}


