using DensfloReport;
using Kitware.VTK;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
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
            vtkPLYReader reader=new vtkPLYReader();
            
            reader.SetFileName("C:\\Users\\dbzho\\OneDrive\\문서\\GitHub\\Field_Training\\vtk_실습용_프로젝트\\upperJaw_1.ply");
            //reader.SetInputData();
            reader.Update();
            //var plypolydata=reader.GetOutput();

            polyData = reader.GetOutput();
            vtkDataArray colors = polyData.GetPointData().GetScalars();

           

            // Create a new array to store the HSV values
            vtkFloatArray hsvValues = new vtkFloatArray();
            hsvValues.SetNumberOfComponents(3);
            hsvValues.SetName("HSVValues");

          

            for (int i = 0; i < colors.GetNumberOfTuples(); i++)
            {
                float r = (float)colors.GetComponent(i, 0);
                float g = (float)colors.GetComponent(i, 1);
                float b = (float)colors.GetComponent(i, 2);

                // Convert RGB to HSV
                Color color = Color.FromArgb((int)r, (int)g, (int)b);
                float hue = color.GetHue();
                float saturation = color.GetSaturation();
                float brightness = color.GetBrightness();

                hsvValues.InsertNextTuple3(hue, saturation, brightness);
            }

           

            // Add the HSV values to the polydata
            polyData.GetPointData().SetScalars(hsvValues);



            //// Scalar values type
            //Console.WriteLine("Scalar values type: " + colors.GetDataTypeAsString());

            //// Print all scalar values
            //for (int i = 0; i < scalarValues.GetNumberOfTuples(); i++)
            //{
            //    float value = (float)scalarValues.GetTuple1(i);
            //    Console.WriteLine("Scalar value " + i + ": " + value);
            //}

            vtkClipPolyData clipper = vtkClipPolyData.New();
            clipper.SetInputData(polyData);
            clipper.SetValue(11);
            clipper.Update();

      

            // Visualize
            vtkPolyDataMapper mapper = new vtkPolyDataMapper();
            mapper.SetInputConnection(clipper.GetOutputPort());
            //mapper.SetInputData(polyData);

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
                콘액터.GetProperty().SetColor(c.R , c.G , c.B );
                콘액터.Modified();
                
            }
        }
    }
}
