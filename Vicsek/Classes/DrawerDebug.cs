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
    class DrawerDebug : DrawerStandart
    {
        public DrawerDebug(PictureBox _pBox) : base(_pBox)
        {

        }

        public override void Draw(IParticle _particle)
        {
            var rect = new Rectangle(particlePoint(_particle).X - 2, particlePoint(_particle).Y - 2, 2, 2);
            
            tempGraph.DrawEllipse(new Pen(Color.Black, 2), rect);
            tempGraph.DrawLine(new Pen(Color.Blue, 2), particlePoint(_particle), particleSpeed(_particle));
        }

        protected override Point particlePoint(IParticle _particle)
        {
            var x = (int)Math.Floor(_particle.CoordinatesInDouble.First);
            var y = (int)Math.Floor(_particle.CoordinatesInDouble.Second);
            return new Point(x, y);
        }

        protected override Point particleSpeed(IParticle _particle)
        {
            var x = (int) Math.Floor(_particle.CoordinatesInDouble.First);
            var y = (int) Math.Floor(_particle.CoordinatesInDouble.Second);

            var vx = (int) Math.Floor(5*Miscelaneous.SpeedDrawMultiplayer*_particle.SpeedInDouble.First);
            var vy = (int) Math.Floor(5*Miscelaneous.SpeedDrawMultiplayer*_particle.SpeedInDouble.Second);

            return new Point(x + vx, y + vy);
        }
    }
}
