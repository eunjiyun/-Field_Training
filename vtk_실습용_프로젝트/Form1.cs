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
            var plypolydata=reader.GetOutput();

            Console.WriteLine(plypolydata/**Directory.GetCurrentDirectory()*/);

            vtkPolyDataMapper mapper=new vtkPolyDataMapper();
            mapper.SetInputData(plypolydata);
            mapper.Update();

            vtkActor actor=new vtkActor();
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
