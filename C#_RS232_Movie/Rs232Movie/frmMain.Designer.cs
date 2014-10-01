namespace Rs232Movie
{
    partial class frmMain
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置 Managed 資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
        /// 修改這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.labelConn = new System.Windows.Forms.Label();
            this.labelConnStatus = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(13, 64);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 1;
            this.button2.Text = "重新連線";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(12, 106);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 2;
            this.button3.Text = "啟動功能";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // labelConn
            // 
            this.labelConn.AutoSize = true;
            this.labelConn.Font = new System.Drawing.Font("新細明體", 14F);
            this.labelConn.Location = new System.Drawing.Point(12, 29);
            this.labelConn.Name = "labelConn";
            this.labelConn.Size = new System.Drawing.Size(85, 19);
            this.labelConn.TabIndex = 3;
            this.labelConn.Text = "是否連線";
            // 
            // labelConnStatus
            // 
            this.labelConnStatus.AutoSize = true;
            this.labelConnStatus.BackColor = System.Drawing.Color.Black;
            this.labelConnStatus.Font = new System.Drawing.Font("新細明體", 14F);
            this.labelConnStatus.ForeColor = System.Drawing.Color.Yellow;
            this.labelConnStatus.Location = new System.Drawing.Point(103, 29);
            this.labelConnStatus.Name = "labelConnStatus";
            this.labelConnStatus.Size = new System.Drawing.Size(28, 19);
            this.labelConnStatus.TabIndex = 4;
            this.labelConnStatus.Text = "否";
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.labelConnStatus);
            this.Controls.Add(this.labelConn);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Name = "frmMain";
            this.Text = "RFID播放程式 Ver 2.0";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label labelConn;
        private System.Windows.Forms.Label labelConnStatus;
    }
}

