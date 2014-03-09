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

            var borderOfArea = new BorderTransit(new List<IPair<double>>
                                            {
                                                new Pair<double>(0, 0),
                                                new Pair<double>(0, _pb.Height),
                                                new Pair<double>(_pb.Width, _pb.Height),
                                                new Pair<double>(_pb.Width, 0)
                                            });

            IParticleBox box = new ParticleBox(factory, 10000, stdDrawer, borderOfArea);

            Run(box);
        }

        private static async void Run(IParticleBox box)
        {
            box.Draw();
            while(true)
            {
                await Task.Run(() => Live(box));
            }
        }


        public static void Live(IParticleBox _box)
        {
            _box.NextStep();
            _box.Draw();
        }
    }
}
