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
        public Form1()
        {
            InitializeComponent();


            this.Load += new EventHandler(Form1_Load); // 이벤트 등록
            this.KeyPreview = true;

            this.WindowState = FormWindowState.Maximized;
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

            for (int i = 0; i < 5; ++i)
                ThreeDViewerInfo.Viewer.Window.interaction.inst.OnMouseWheelBackward();

            Random rand = new Random(); // Random 객체 생성
            double num = rand.NextDouble() * 1.5 - 0.75; // -1 이상 1 이하의 랜덤한 실수 생성
            double num2 = rand.NextDouble() * 1.5 - 0.75; // -1 이상 1 이하의 랜덤한 실수 생성
            double num3 = rand.NextDouble() * 1.5 - 0.75; // -1 이상 1 이하의 랜덤한 실수 생성


            vtkConeSource coneSource = new vtkConeSource();


            // Create a sphere.  
            vtkSphereSource sphereSource = vtkSphereSource.New();//cpp에서 많이 쓰는 문법
            sphereSource.SetRadius(0.5);//sphereSource.으로 속성에 접근할 수 있다
            sphereSource.SetPhiResolution(50);
            sphereSource.SetThetaResolution(50);//여기까지가 데이터에 해당하는 부분 


            // Visualize
            vtkPolyDataMapper mapper = vtkPolyDataMapper.New();//데이터를 어떤 형태로 시각화 할지
            //폴리데이터 : 면들로 이루어진 부분
            //매퍼의 종류중 vtk매퍼중 
            mapper.SetInputConnection(sphereSource.GetOutputPort());


            vtkPolyDataMapper conemapper = vtkPolyDataMapper.New();//데이터를 어떤 형태로 시각화 할지
            conemapper.SetInputConnection(coneSource.GetOutputPort());

            //vtkActor actor = vtkActor.New();
            //actor.SetMapper(mapper);
            ////매퍼에 데이터 세팅->액터에 매퍼를 세팅
            //actor.SetScale(0.1f);

            vtkActor actor2 = vtkActor.New();
            actor2.SetMapper(mapper);
            actor2.SetScale(0.5f);
            actor2.SetPosition(0.03f + num, 0 + num2, 0 + num3);
            actor2.SetObjectName("안녕");
            // vtkProperty prop=actor2.GetProperty();
            //  prop.SetColor(1, 0, 0);

            vtkProperty prop = actor2.GetProperty();

            double[] rgb2 = new double[3];
            vtkNamedColors.New().GetColor("Cyan", ref rgb2[0], ref rgb2[1], ref rgb2[2]);

            prop.SetColor(rgb2[0], rgb2[1], rgb2[2]);

            vtkActor conActor = vtkActor.New();
            conActor.SetMapper(conemapper);
            conActor.SetScale(0.5f);
            conActor.SetPosition(0.0f + num, -0.39f + num2, 0 + num3);
            conActor.RotateZ(-Right);
            vtkProperty prop2 = conActor.GetProperty();

            콘액터 = conActor;

            double[] rgb = new double[3];
            vtkNamedColors.New().GetColor("Gold", ref rgb[0], ref rgb[1], ref rgb[2]);

            prop2.SetColor(rgb[0], rgb[1], rgb[2]);



            vtkRenderWindow renderWindow = renderWindowControl1.RenderWindow;
            vtkRenderer renderer = renderWindow.GetRenderers().GetFirstRenderer();
            // renderer.SetBackground(0.3, 0.2, 0.1);
            // renderer.AddActor(actor);
            renderer.AddActor(actor2);
            renderer.AddActor(conActor);
            //렌더러에 액터를 세팅

            //cg와 연관된 부분 컴그배웠으면 이해하기 수월




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
