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
        public void TestIntersectionAngle()
        {
            var A = new Pair<double>(0, 1);
            var B = new Pair<double>(2, 0);
            var C = new Pair<double>(1, 0);
            var D = new Pair<double>(1, 2);

            var ang = Miscelaneous.GetDegreeBetveen(A, B, C, D);

            Assert.AreEqual(ang, 45);
        }

    }
}

