namespace Rs232Movie
{
    partial class DisplayFuncForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DisplayFuncForm));
            this.WMP1 = new AxWMPLib.AxWindowsMediaPlayer();
            ((System.ComponentModel.ISupportInitialize)(this.WMP1)).BeginInit();
            this.SuspendLayout();
            // 
            // WMP1
            // 
            this.WMP1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.WMP1.Enabled = true;
            this.WMP1.Location = new System.Drawing.Point(0, 0);
            this.WMP1.Name = "WMP1";
            this.WMP1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("WMP1.OcxState")));
            this.WMP1.Size = new System.Drawing.Size(422, 373);
            this.WMP1.TabIndex = 1;
            // 
            // DisplayFuncForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(422, 373);
            this.Controls.Add(this.WMP1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.KeyPreview = true;
            this.Name = "DisplayFuncForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "DisplayFuncForm";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.DisplayFuncForm_KeyDown);
            ((System.ComponentModel.ISupportInitialize)(this.WMP1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxWMPLib.AxWindowsMediaPlayer WMP1;

    }
}