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
        private static IParticleFactory2D m_tstFactory;

        private static PictureBox m_pBox;

        public static void Init(PictureBox _pb)
        {
            IDrawer stdDrawer = new DrawerStandart(_pb);
            IDrawer testDrawer = new DrawerDebug(_pb);
            IDrawer interstingDrawer = new DrawerInteresting(_pb);
            m_pBox = _pb;


            m_factory2 = new ParticleFactoryStandart(_pb.Width, _pb.Height, stdDrawer);

            m_tstFactory = new ParticleFactoryInteresting(_pb.Width, _pb.Height, interstingDrawer);


            var borderOfArea = new BorderTransit(new List<PairDouble>
                                            {
                                                new PairDouble(0, 0),
                                                new PairDouble(0, _pb.Height),
                                                new PairDouble(_pb.Width, _pb.Height),
                                                new PairDouble(_pb.Width, 0)
                                            });

            m_box = new ParticleBox(m_factory2, Miscelaneous.ParticleCount, stdDrawer, borderOfArea);

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

        public static void AddParticles(int count)
        {
            lock (m_box)
            {
                m_box.AddParticles(m_factory2, count);
            }
        }

        public static void AddParticlesInteresting(int count)
        {
            lock (m_box)
            {
                m_box.AddParticles(m_tstFactory, count);
            }
        }

        public static void Live(IParticleBox _box)
        {
            lock (_box)
            {
                _box.NextStep();
                _box.Draw();
                _box.Particles.Last().Draw(m_pBox);
            }
        }
    }
}
