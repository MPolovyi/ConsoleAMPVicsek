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
        private static IParticleBox m_box;
        private static IParticleFactory2D m_factory2;
        private static IParticleFactory2D m_TstFactory;

        public static void Init(int _steps, PictureBox _pb)
        {
            IDrawer stdDrawer = new DrawerStandart(_pb);
            IDrawer testDrawer = new DrawerDebug(_pb);

            m_factory2 = new ParticleFactoryStandart(_pb.Width, _pb.Height, stdDrawer);

            m_TstFactory = new ParticleFactoryStandart(_pb.Width, _pb.Height, testDrawer);


            var borderOfArea = new BorderTransit(new List<PairDouble>
                                            {
                                                new PairDouble(0, 0),
                                                new PairDouble(0, _pb.Height),
                                                new PairDouble(_pb.Width, _pb.Height),
                                                new PairDouble(_pb.Width, 0)
                                            });

            m_box = new ParticleBox(m_factory2, 100, stdDrawer, borderOfArea);

            Run(m_box);
        }

        private static async void Run(IParticleBox _box)
        {
            _box.Draw();
            while(true)
            {
                await Task.Run(() => Live(_box));
            }
        }

        public static void AddParticles()
        {
            m_box.AddParticles(m_factory2, 100);
        }

        public static void Live(IParticleBox _box)
        {
            _box.NextStep();
            _box.Draw();
        }
    }
}
