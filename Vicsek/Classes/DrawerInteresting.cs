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
    class DrawerInteresting : DrawerStandart
    {
        public DrawerInteresting(PictureBox _pBox) : base(_pBox)
        {
        }

        public override void Draw(IParticle _particle)
        {
            var rect = new Rectangle(particlePoint(_particle).X - 2, particlePoint(_particle).Y - 2, 4, 4);
            tempGraph.FillEllipse(new SolidBrush(Color.GreenYellow), rect);
            tempGraph.DrawLine(new Pen(Color.Green, 2), particlePoint(_particle), particleSpeed(_particle));

        }
    }
}
