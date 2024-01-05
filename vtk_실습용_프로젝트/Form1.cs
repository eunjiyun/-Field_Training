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


            for (int i = 0; i < 20; ++i)
                ThreeDViewerInfo.Viewer.Window.interaction.inst.OnMouseWheelBackward();




            vtkRenderWindow renderWindow = renderWindowControl1.RenderWindow;
            vtkRenderer renderer = renderWindow.GetRenderers().GetFirstRenderer();


            vtkConeSource coneSource = new vtkConeSource();
            vtkSphereSource sphereSource = vtkSphereSource.New();
            sphereSource.SetRadius(0.5);
            sphereSource.SetPhiResolution(5);
            sphereSource.SetThetaResolution(5);

            vtkGlyph3D glyph3D = new vtkGlyph3D();
            glyph3D.SetSourceConnection(sphereSource.GetOutputPort());

            vtkGlyph3D coneGlyph3D = new vtkGlyph3D();
            coneGlyph3D.SetSourceConnection(coneSource.GetOutputPort());

            //List<float> numList = new List<float>();
            List<float> numList = new List<float>(1000000); // Initialize with the known size.
            Random rand = new Random();

            for (int i = 0; i < 1000000; ++i) // Test with 10, but can be changed to 1,000,000
            {
                float num = (float)rand.NextDouble() * 26 - 13;
                numList.Add(num);
            }

            int chunkSize = 1000; // 한 번에 처리할 데이터의 수를 설정합니다.
            int loopCount = (int)Math.Ceiling((double)numList.Count / chunkSize); // 전체 루프 실행 횟수를 계산합니다.

            vtkPoints points = new vtkPoints();
            vtkPoints conePoints = new vtkPoints();



            for (int loopIndex = 0; loopIndex < loopCount; loopIndex++)
            {
                int currentChunkSize = Math.Min(chunkSize, numList.Count - loopIndex * chunkSize); // 현재 청크의 크기를 계산합니다.


                for (int i = 0; i < currentChunkSize; ++i) // numList의 일부분만 처리합니다.
                {
                    double numValue = numList[loopIndex * chunkSize + i];


                    points.InsertNextPoint(0.03f + numValue, 0 + numValue, 0 + numValue);
                    conePoints.InsertNextPoint(0.8f + numValue, numValue, 0 + numValue);

                }
            }

            vtkPolyData spheresData = new vtkPolyData();
            spheresData.SetPoints(points);
            glyph3D.SetInputData(spheresData);
            glyph3D.ScalingOff();
            //glyph3D.Update();

            vtkPolyData conesData = new vtkPolyData();
            conesData.SetPoints(conePoints);
            coneGlyph3D.SetInputData(conesData);
            coneGlyph3D.ScalingOff();
            //coneGlyph3D.Update();

            vtkPolyDataMapper mapper = vtkPolyDataMapper.New();
            mapper.SetInputConnection(glyph3D.GetOutputPort());

            vtkPolyDataMapper coneMapper = vtkPolyDataMapper.New();
            coneMapper.SetInputConnection(coneGlyph3D.GetOutputPort());

            vtkLODActor actor = vtkLODActor.New();
            actor.SetMapper(mapper);
            actor.GetProperty().SetColor(0, 1, 1); // Cyan
            actor.SetNumberOfCloudPoints(100); // Set the number of points for the lowest resolution 

            vtkLODActor coneActor = vtkLODActor.New();
            coneActor.SetMapper(coneMapper);
            coneActor.GetProperty().SetColor(1, 0.843137, 0); // Gold
            coneActor.SetNumberOfCloudPoints(100); // Set the number of points for the lowest resolution 
            콘액터 = coneActor;
            renderer.AddActor(actor);
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
                콘액터.GetProperty().SetColor(c.R / 255f, c.G / 255f, c.B / 255f);
                콘액터.Modified();
            }
        }
    }
}
