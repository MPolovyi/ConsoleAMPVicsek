using System;
using System.Collections.Generic;
using Microsoft.QualityTools.Testing.Fakes;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Vicsek;
using Vicsek.Classes;
using Vicsek.Classes.Fakes;
using Vicsek.Interfaces;
using Vicsek.Interfaces.Fakes;


namespace UnitTestProject1
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            Pair<double> a1 = new Pair<double>(0, 0);
            Pair<double> b1 = new Pair<double>(5, 2);
            Pair<double> a2 = new Pair<double>(2, 0);
            Pair<double> b2 = new Pair<double>(0, 5);

            bool areIntersectExpected = true;

            bool areIntersect = Miscelaneous.Intersect(a1, b1, a2, b2);

            Assert.AreEqual(areIntersectExpected, areIntersect);
        }

        [TestMethod]
        public void TestParticleReflect()
        {
            IParticle pt = new StubIParticle()
                {
                    CoordinatesInDoubleGet = () => new Pair<double>(5,5),
                    SpeedInDoubleGet = () => new Pair<double>(-4,-15)
                };
            
            IBorder border = new StubIBorder()
                {
                    CornersGet = () => new List<IPair<double>> {new Pair<double>(0, 0), new Pair<double>(10, 0)},

                    CheckIParticle = (_particle) =>
                        {
                            var corners = new List<IPair<double>>
                                {
                                    new Pair<double>(0, 0),
                                    new Pair<double>(10, 0)
                                };
                            for (int i = 0; i < corners.Count - 1; i++)
                            {
                                var pt1 = _particle.CoordinatesInDouble;
                                var pt2 = _particle.SpeedInDouble + pt1;

                                var pt3 = corners[i];
                                var pt4 = corners[i + 1];

                                var isIntersection = Miscelaneous.Intersect(pt1, pt2, (Pair<double>) pt3,
                                                                            (Pair<double>) pt4);

                                if (isIntersection)
                                {
                                    (_particle).UpdSpeed();
                                    return true;
                                }
                            }
                            return false;
                        }
                };

            bool expectedIntersection1 = true;
            bool actualIntersection1 = border.Check(pt);

            Assert.AreEqual(expectedIntersection1, actualIntersection1);
        }

        [TestMethod]
        public void TestParticleDontReflect()
        {
            IParticle pt = new StubIParticle()
            {
                CoordinatesInDoubleGet = () => new Pair<double>(5, 5),
                SpeedInDoubleGet = () => new Pair<double>(0, 2)
            };

            IBorder border = new StubIBorder()
            {
                CornersGet = () => new List<IPair<double>> { new Pair<double>(0, 10), new Pair<double>(10, 10) },

                CheckIParticle = (_particle) =>
                {
                    var corners = new List<IPair<double>>
                                {
                                    new Pair<double>(0, 10),
                                    new Pair<double>(10, 10)
                                };
                    for (int i = 0; i < corners.Count - 1; i++)
                    {
                        var pt1 = _particle.CoordinatesInDouble;
                        var pt2 = _particle.SpeedInDouble + pt1;

                        var pt3 = corners[i];
                        var pt4 = corners[i + 1];

                        var isIntersection = Miscelaneous.Intersect(pt1, pt2, (Pair<double>)pt3,
                                                                    (Pair<double>)pt4);

                        if (isIntersection)
                        {
                            (_particle).UpdSpeed();
                            return true;
                        }
                    }
                    return false;
                }
            };

            bool expectedIntersection1 = false;
            bool actualIntersection1 = border.Check(pt);

            Assert.AreEqual(expectedIntersection1, actualIntersection1);
        }
    }
}

