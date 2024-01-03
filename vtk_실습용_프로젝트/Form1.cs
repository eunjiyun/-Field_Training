using DensfloReport;
using Kitware.VTK;
using System.Windows.Forms;

namespace vtk_실습용_프로젝트
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void renderWindowControl1_Load(object sender, System.EventArgs e)
        {
            //초기화 하는 함수
            ThreeDViewerInfo.Viewer.Initalize(sender as RenderWindowControl);
            ThreeDViewerInfo.InitCustomInteraction(ThreeDViewerInfo.Viewer);
        }
    }
}
