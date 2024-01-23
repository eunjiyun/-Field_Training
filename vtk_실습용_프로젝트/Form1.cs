using DensfloReport;
using Kitware.VTK;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace vtk_실습용_프로젝트
{
    public partial class Form1 : Form
    {
        vtkActor 콘액터;
        public Form1()
        {
            InitializeComponent();


            this.Load += new EventHandler(Form1_Load); // 이벤트 등록
            this.KeyPreview = true;

            //this.WindowState = FormWindowState.Maximized;
            this.FormBorderStyle = FormBorderStyle.None;

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Focus();
        }



        private void renderWindowControl1_Load(object sender, System.EventArgs e)
        {

            //초기화 하는 함수
            ThreeDViewerInfo.Viewer.Initalize(sender as RenderWindowControl);
            ThreeDViewerInfo.InitCustomInteraction(ThreeDViewerInfo.Viewer);

            vtkRenderWindow renderWindow = renderWindowControl1.RenderWindow;
            vtkRenderer renderer = renderWindow.GetRenderers().GetFirstRenderer();


            // For coneActor Color
            Color coneActorColor;
            // For creamActor Color
            Color creamActorColor;
          

            // Define colors
            coneActorColor = Color.FromArgb(210, 105, 30); // RGB for "chocolate"
            creamActorColor = Color.FromArgb(189, 252, 201); // RGB for "mint"
           
            vtkCone cone = new vtkCone();
            cone.SetAngle(20);

            vtkPlane vertPlane = new vtkPlane();
            vertPlane.SetOrigin(.1, 0, 0);
            vertPlane.SetNormal(-1, 0, 0);

            vtkPlane basePlane = new vtkPlane();
            basePlane.SetOrigin(1.2, 0, 0);
            basePlane.SetNormal(1, 0, 0);

            vtkSphere iceCream = new vtkSphere();
            iceCream.SetCenter(1.333, 0, 0);
            iceCream.SetRadius(0.5);

            vtkSphere bite = new vtkSphere();
            bite.SetCenter(1.5, 0, 0.5);
            bite.SetRadius(0.25);

            // Combine primitives to build ice-cream cone. Clip the cone with planes.
            vtkImplicitBoolean theCone = new vtkImplicitBoolean();
            theCone.SetOperationTypeToIntersection();
            theCone.AddFunction(cone);
            theCone.AddFunction(vertPlane);
            theCone.AddFunction(basePlane);

            // Take a bite out of the ice cream.
            vtkImplicitBoolean theCream = new vtkImplicitBoolean();
            theCream.SetOperationTypeToDifference();
            theCream.AddFunction(iceCream);
            theCream.AddFunction(bite);

            // The sample function generates a distance function from the
            // implicit function (which in this case is the cone). 
            // This is then contoured to get a polygonal surface.

            vtkSampleFunction theConeSample = new vtkSampleFunction();
            theConeSample.SetImplicitFunction(theCone);
            theConeSample.SetModelBounds(-1, 1.5, -1.25, 1.25, -1.25, 1.25);
            theConeSample.SetSampleDimensions(128, 128, 128);
            theConeSample.ComputeNormalsOff();

            vtkContourFilter theConeSurface = new vtkContourFilter();
            theConeSurface.SetInputConnection(theConeSample.GetOutputPort());
            theConeSurface.SetValue(0, 0.0);

            vtkPolyDataMapper coneMapper = new vtkPolyDataMapper();
            coneMapper.SetInputConnection(theConeSurface.GetOutputPort());
            coneMapper.ScalarVisibilityOff();

            vtkActor coneActor = new vtkActor();
            coneActor.SetMapper(coneMapper);
            coneActor.GetProperty().SetColor(coneActorColor.R / 255.0f, coneActorColor.G / 255.0f, coneActorColor.B / 255.0f);
            //콘액터.GetProperty().SetColor(c.R / 255f, c.G / 255f, c.B / 255f);
            콘액터 = coneActor;
            //The same here for the ice cream.

            vtkSampleFunction theCreamSample = new vtkSampleFunction();
            theCreamSample.SetImplicitFunction(theCream);
            theCreamSample.SetModelBounds(0, 2.5, -1.25, 1.25, -1.25, 1.25);
            theCreamSample.SetSampleDimensions(128, 128, 128);
            theCreamSample.ComputeNormalsOff();

            vtkContourFilter theCreamSurface = new vtkContourFilter();
            theCreamSurface.SetInputConnection(theCreamSample.GetOutputPort());
            theCreamSurface.SetValue(0, 0.0);

            vtkPolyDataMapper creamMapper = new vtkPolyDataMapper();
            creamMapper.SetInputConnection(theCreamSurface.GetOutputPort());
            creamMapper.ScalarVisibilityOff();

            vtkActor creamActor = new vtkActor();
            creamActor.SetMapper(creamMapper);
            creamActor.GetProperty().SetDiffuseColor(creamActorColor.R/255.0f , creamActorColor.G / 255.0f, creamActorColor.B / 255.0f);
            creamActor.GetProperty().SetSpecular(.6);
            creamActor.GetProperty().SetSpecularPower(50);

            

            renderer.AddActor(creamActor);
            renderer.AddActor(coneActor);
        }


        private void reset(object sender, System.EventArgs e)
        {

            Application.Restart(); // 프로그램 재시작
        }



        private void exit(object sender, EventArgs e)
        {
            this.Close();
        }

        private void play(object sender, EventArgs e)
        {
            textBox1.Text = "start!";
        }


        private void coneColor(object sender, EventArgs e)
        {
            ColorDialog coldial = new ColorDialog();

            if (coldial.ShowDialog() == DialogResult.OK)
            {
                //button4.BackColor = coldial.Color;

                Color c = coldial.Color;
                콘액터.GetProperty().SetColor(c.R , c.G , c.B );
                콘액터.Modified();
                
            }
        }
    }
}
