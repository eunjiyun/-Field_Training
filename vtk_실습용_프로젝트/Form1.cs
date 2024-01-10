using DensfloReport;
using Kitware.VTK;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace vtk_실습용_프로젝트
{
    public partial class Form1 : Form
    {
        vtkActor 콘액터;
        public vtkPolyData polyData;
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
            //readply 
            vtkPLYReader reader = new vtkPLYReader();

            reader.SetFileName("C:\\Users\\dbzho\\OneDrive\\문서\\GitHub\\Field_Training\\vtk_실습용_프로젝트\\upperJaw_1.ply");
            reader.Update();


            polyData = reader.GetOutput();

            // 클리핑 전 RGB 색상 정보를 저장합니다.
            vtkUnsignedCharArray originalColors = polyData.GetPointData().GetScalars() as vtkUnsignedCharArray;


            // 클리핑된 폴리데이터의 각 점에 대한 새로운 RGB 색상 배열을 생성합니다.
            vtkUnsignedCharArray clippedColors = vtkUnsignedCharArray.New();
            clippedColors.SetNumberOfComponents(3); // R, G, B
            clippedColors.SetName("Colors");

            // Create a new array to store the HSV values
            vtkFloatArray hsvValues = new vtkFloatArray();
            hsvValues.SetNumberOfComponents(3);
            hsvValues.SetName("HSVValues");



            for (int i = 0; i < originalColors.GetNumberOfTuples(); ++i)
            {
                float r = (float)originalColors.GetComponent(i, 0);
                float g = (float)originalColors.GetComponent(i, 1);
                float b = (float)originalColors.GetComponent(i, 2);

                // Convert RGB to HSV
                Color color = Color.FromArgb((int)r, (int)g, (int)b);
                float hue = color.GetHue();
                float saturation = color.GetSaturation();
                float brightness = color.GetBrightness();

                // 변환된 RGB 값을 vtkFloatArray에 추가합니다.
                hsvValues.InsertNextTuple3(hue, saturation, brightness);
            }

            polyData.GetPointData().SetScalars(hsvValues);


            // 클리핑을 수행합니다.
            vtkClipPolyData clipper = vtkClipPolyData.New();
            clipper.SetInputData(polyData);

            clipper.SetValue(11);
            clipper.Update();

            //// 클리핑된 폴리데이터를 가져옵니다.
            polyData = clipper.GetOutput();


            // 클리핑된 폴리데이터의 각 점에 대해 가장 가까운 원본 점의 색상을 매핑합니다.
            for (int i = 0; i < polyData.GetNumberOfPoints(); ++i)
                clippedColors.InsertNextTuple3(255, 255, 255);


            // 클리핑된 폴리데이터에 매핑된 색상 정보를 설정합니다.
            polyData.GetPointData().SetScalars(clippedColors);


            // Visualize
            vtkPolyDataMapper mapper = new vtkPolyDataMapper();
            mapper.SetInputData(polyData);

            vtkActor actor = new vtkActor();
            actor.SetMapper(mapper);


            renderWindowControl1.RenderWindow.GetRenderers().GetFirstRenderer().AddActor(actor);

            //초기화 하는 함수
            ThreeDViewerInfo.Viewer.Initalize(sender as RenderWindowControl);
            ThreeDViewerInfo.InitCustomInteraction(ThreeDViewerInfo.Viewer);
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
