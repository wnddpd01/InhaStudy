namespace CWSMapTool
{
    partial class Form1
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
            this.panel_PreviewMap = new System.Windows.Forms.Panel();
            this.panel_tile_select = new System.Windows.Forms.Panel();
            this.label_block_tile = new System.Windows.Forms.Label();
            this.panel_tile_select.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel_PreviewMap
            // 
            this.panel_PreviewMap.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.panel_PreviewMap.Cursor = System.Windows.Forms.Cursors.Default;
            this.panel_PreviewMap.Enabled = false;
            this.panel_PreviewMap.Location = new System.Drawing.Point(10, 10);
            this.panel_PreviewMap.Margin = new System.Windows.Forms.Padding(0);
            this.panel_PreviewMap.Name = "panel_PreviewMap";
            this.panel_PreviewMap.Size = new System.Drawing.Size(960, 540);
            this.panel_PreviewMap.TabIndex = 0;
            // 
            // panel_tile_select
            // 
            this.panel_tile_select.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel_tile_select.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel_tile_select.Controls.Add(this.label_block_tile);
            this.panel_tile_select.Location = new System.Drawing.Point(1000, 50);
            this.panel_tile_select.Name = "panel_tile_select";
            this.panel_tile_select.Size = new System.Drawing.Size(450, 200);
            this.panel_tile_select.TabIndex = 0;
            this.panel_tile_select.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            // 
            // label_block_tile
            // 
            this.label_block_tile.AutoSize = true;
            this.label_block_tile.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label_block_tile.Location = new System.Drawing.Point(4, 4);
            this.label_block_tile.Name = "label_block_tile";
            this.label_block_tile.Size = new System.Drawing.Size(45, 12);
            this.label_block_tile.TabIndex = 0;
            this.label_block_tile.Text = "벽 타일";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1464, 561);
            this.Controls.Add(this.panel_tile_select);
            this.Controls.Add(this.panel_PreviewMap);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            this.panel_tile_select.ResumeLayout(false);
            this.panel_tile_select.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panel_tile_select;
        private System.Windows.Forms.Label label_block_tile;
        private System.Windows.Forms.Panel panel_PreviewMap;
    }
}

