using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace CWSMapTool
{
    public partial class Form1 : Form
    {
        const uint horizontalGridCount = 32;
        const uint verticalGridCount = 18;
        PictureBox selectedTile = null;
        Bitmap bitmap_blank = new Bitmap(@".\Image\Tile\cws_tile_blank.png");
        Dictionary<string, Bitmap> dic_bitmap_tile = new Dictionary<string, Bitmap>();
        int tileWidth = 0;
        int tileHeight = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            PreviewMap_Create();
            BlockTile_Select_Create();
            selectedTile = new PictureBox();
            Controls.Add(selectedTile);
            selectedTile.BringToFront();
            selectedTile.Visible = false;
            panel_tile_select.SendToBack();
            this.BringToFront();
        }
        
        private void PreviewMap_Create()
        {
            tileWidth  = (int)(panel_PreviewMap.Width / horizontalGridCount);
            tileHeight = (int)(panel_PreviewMap.Height / verticalGridCount);
            for (int i = 0; i < verticalGridCount; i++)
            {
                for (int j = 0; j < horizontalGridCount; j++)
                {
                    PictureBox p1 = new PictureBox();
                    p1.Left = j * tileWidth;
                    p1.Top = i * tileHeight;
                    p1.Width = tileWidth;
                    p1.Height = tileHeight;
                    p1.SizeMode = PictureBoxSizeMode.StretchImage;
                    p1.Image = bitmap_blank;
                    panel_PreviewMap.Controls.Add(p1);
                }
            }
        }

        private void BlockTile_Select_Create()
        {
            dic_bitmap_tile.Add("BasicTile" ,new Bitmap(@".\Image\Tile\cws_tile_basic.bmp"));
            dic_bitmap_tile.Add("RedTile", new Bitmap(@".\Image\Tile\cws_tile_red.bmp"));
            dic_bitmap_tile.Add("PinkTile", new Bitmap(@".\Image\Tile\cws_tile_pink.bmp"));
            dic_bitmap_tile.Add("BrickTile", new Bitmap(@".\Image\Tile\cws_tile_brick.bmp"));

            int cnt = 0;
            foreach(KeyValuePair<string, Bitmap> bitmap in dic_bitmap_tile)
            {
                PictureBox p_tileBasic = new PictureBox();
                p_tileBasic.Left = 10 + cnt * 48;
                p_tileBasic.Top = 30;
                p_tileBasic.Width = 48;
                p_tileBasic.Height = 48;
                p_tileBasic.SizeMode = PictureBoxSizeMode.StretchImage;
                p_tileBasic.Name = bitmap.Key;
                p_tileBasic.Image = bitmap.Value;
                p_tileBasic.MouseClick += new MouseEventHandler(TileClick);
                panel_tile_select.Controls.Add(p_tileBasic);
                cnt++;
            }
        }

        public void TileClick(object sender, MouseEventArgs arg)
        {
            PictureBox pic = sender as PictureBox;
            if(selectedTile.Image == null)
            {
                selectedTile.Image = pic.Image;
                selectedTile.Width = pic.Width;
                selectedTile.Height = pic.Height;
                selectedTile.Visible = true;
            }
        }

        public void Form1_MouseMove(object sender, MouseEventArgs arg)
        {
            if (selectedTile.Image != null)
            {
                selectedTile.Location = new Point(arg.X, arg.Y);
            }   
        }
    }
}
