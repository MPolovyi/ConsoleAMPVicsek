using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class BorderTransit : Border
    {
        private double m_SizeX, m_SizeY;
        public BorderTransit(List<IPair<double>> _corners) : base(_corners)
        {
            m_SizeX = _corners[2].First;
            m_SizeY = _corners[2].Second;
        }

        public override void Interract(IParticle _particle, int _index)
        {
            var transit = 0;
          
            Pair<double> newPos = new Pair<double>(0,0);
            if (_index == 0)
            {
                newPos = _particle.CoordinatesInDouble +  new Pair<double>(m_SizeX, 0);
            }
            if (_index == 1)
            {
                newPos = _particle.CoordinatesInDouble - new Pair<double>(0, m_SizeY);
            }
            if (_index == 2)
            {
                newPos = _particle.CoordinatesInDouble - new Pair<double>(m_SizeX, 0);
            }
            if (_index==3)
            {
                newPos = _particle.CoordinatesInDouble + new Pair<double>(0, m_SizeY);
            }
            _particle.UpdCoordinates(newPos + _particle.SpeedInDouble);
        }
    }
}
