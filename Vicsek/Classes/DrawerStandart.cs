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
        protected PictureBox pBox { get; set; }
        protected int SizeX { get; set; }
        protected int SizeY { get; set; }
        protected static Graphics Graph { get; set; }
        protected static Graphics tempGraph { get; set; }

        protected Pen PenBlack { get; set; }
        protected Pen PenBlue { get; set; }
        protected Brush BrushWhite { get; set; }

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
            var rect = new Rectangle(particlePoint(_particle).X - 1, particlePoint(_particle).Y - 1, 2, 2);

            tempGraph.DrawEllipse(new Pen(Color.Black, 2), rect);
            tempGraph.DrawLine(new Pen(Color.Blue, 2), particlePoint(_particle), particleSpeed(_particle));
        }

        protected virtual Point particlePoint(IParticle _particle)
        {
            var x = (int)Math.Floor(_particle.CoordinatesInDouble.First);
            var y = (int)Math.Floor(_particle.CoordinatesInDouble.Second);
            return new Point(x, y);
        }

        protected virtual Point particleSpeed(IParticle _particle)
        {
            var x = (int)Math.Floor(_particle.CoordinatesInDouble.First);
            var y = (int)Math.Floor(_particle.CoordinatesInDouble.Second);

            var vx = (int)Math.Floor(Miscelaneous.SpeedDrawMultiplayer * _particle.SpeedInDouble.First);
            var vy = (int)Math.Floor(Miscelaneous.SpeedDrawMultiplayer * _particle.SpeedInDouble.Second);

            return new Point(x + vx, y + vy);
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
