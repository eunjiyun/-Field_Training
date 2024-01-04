using System.Windows.Forms;

namespace vtk_실습용_프로젝트
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.renderWindowControl1 = new Kitware.VTK.RenderWindowControl();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // renderWindowControl1
            // 
            this.renderWindowControl1.AddTestActors = false;
            this.renderWindowControl1.Location = new System.Drawing.Point(0, 13);
            this.renderWindowControl1.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.renderWindowControl1.Name = "renderWindowControl1";
            this.renderWindowControl1.Size = new System.Drawing.Size(1750, 1070);
            this.renderWindowControl1.TabIndex = 0;
            this.renderWindowControl1.TestText = null;
            this.renderWindowControl1.Load += new System.EventHandler(this.renderWindowControl1_Load);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1477, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 29);
            this.button1.TabIndex = 1;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.ButtonClick);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(1477, 47);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(75, 25);
            this.textBox1.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1920, 1055);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.renderWindowControl1);
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

       
        #endregion

        private Kitware.VTK.RenderWindowControl renderWindowControl1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
    }


}

