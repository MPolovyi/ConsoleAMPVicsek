using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class DrawerStandart : IDrawer
    {
        private PictureBox pBox { get; set; }
        private int SizeX { get; set; }
        private int SizeY { get; set; }
        private Graphics Graph { get; set; }
        private Graphics tempGraph { get; set; }

        private Pen PenBlack { get; set; }
        private Pen PenBlue { get; set; }
        private Brush BrushWhite { get; set; }

        public DrawerStandart(PictureBox _pBox)
        {
            pBox = _pBox;

            SizeX = pBox.Width;
            SizeY = pBox.Height;

            Graph = pBox.CreateGraphics();

            PenBlack = new Pen(Color.Black, 2);
            PenBlue = new Pen(Color.Blue, 2);

            BrushWhite = new SolidBrush(Color.White);
        }

        public virtual void Draw(IParticle _particle)
        {
            var rect = new Rectangle(_particle.CoordinatesInPoint.X - 3, _particle.CoordinatesInPoint.Y - 3, 6, 6);

            tempGraph.DrawEllipse(new Pen(Color.Black, 2), rect);
            tempGraph.DrawLine(new Pen(Color.Blue, 2), _particle.CoordinatesInPoint, _particle.SpeedInPoint);
        }

        public virtual void Draw(IParticleBox _box)
        {
            var bmp = new Bitmap(SizeX, SizeY, Graph);

            tempGraph = Graphics.FromImage(bmp);
            tempGraph.Clear(Color.White);

            foreach (var particle in _box.Particles)
            {
                particle.Draw();
            }
            
            Graph.DrawImage(bmp, 0, 0);
        }
    }
}
