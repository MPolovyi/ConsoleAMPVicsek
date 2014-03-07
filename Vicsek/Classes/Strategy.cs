using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    static class Strategy
    {
        public static void Init(int _steps, PictureBox _pb)
        {
            IDrawer stdDrawer = new DrawerStandart(_pb);

            IParticleFactory2D factory = new ParticleFactoryStandart(_pb.Width, _pb.Height, stdDrawer);

            IParticleBox box = new ParticleBox(factory, 100, stdDrawer);

            box.Draw();
            for (int i = 0; i < 1000; i++)
            {
                Live(box);
            }
            
        }

        public static void Live(IParticleBox _box)
        {
            _box.NextStep();
            _box.Draw();
        }
    }
}
