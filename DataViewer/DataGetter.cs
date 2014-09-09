using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace DataViewer
{
    internal class DataGetter
    {
        public string GetSimParams(StreamReader fs)
        {
            var str = fs.ReadLine();
            fs.BaseStream.Seek(0, SeekOrigin.Begin);
            return str;
        }

        public List<Tuple<double, double>> GetSpeedByNoise(StreamReader fs)
        {
            List<string> strs = new List<string>();
            Regex dataRegex = new Regex(@"y = \d");
            while (!fs.EndOfStream)
            {
                var str = fs.ReadLine();
                if (dataRegex.IsMatch(str))
                {
                    strs.Add(str);
                }
            }
            var velocAndNoise = new List<Tuple<double, double>>(strs.Count);
            Regex notNumbers = new Regex(@"[A-Za-z]");
            Regex dotRegex = new Regex(@"\.");
            for (int i = 0; i < strs.Count; i++)
            {
                string veloc = dotRegex.Replace(notNumbers.Replace(strs[i].Split('=')[1], ""), ",");
                string noise = dotRegex.Replace(notNumbers.Replace(strs[i].Split('=')[2], ""), ",");

                velocAndNoise.Add(new Tuple<double, double>(
                    Convert.ToDouble(noise),
                    Convert.ToDouble(veloc)));
            }
            fs.BaseStream.Seek(0, SeekOrigin.Begin);

            return velocAndNoise;
        }

        public List<Tuple<List<double>, double>> GetSpeedByHeight(StreamReader fs)
        {
            var strs = new List<string>();
            while (!fs.EndOfStream)
            {
                Regex dataRegex = new Regex(@"Noise = \d");
                Regex notNumbers = new Regex(@"[A-Za-z=]");
                Regex dotRegex = new Regex(@"\.");
                var str = fs.ReadLine();
                if (str != null && dataRegex.IsMatch(str))
                {
                    strs.Add(dotRegex.Replace(notNumbers.Replace(str, ""), ","));
                }
            }

            var ret = new List<Tuple<List<double>, double>>();
            foreach (var str in strs)
            {
                var tmp = Regex.Replace(str, @"\s+", " ").Split(' ');
                var tempList = new List<string>(tmp);
                var tempDouble = Convert.ToDouble(tempList.Last());
                tempList.Remove(tempList.Last());
                try
                {
                    ret.Insert(0, new Tuple<List<double>, double>(tempList.ConvertAll(Convert.ToDouble), tempDouble));
                }
                catch (FormatException)
                {
                    List<double> tmpDoubleList = new List<double>(tempList.Count);
                    foreach (var str1 in tempList)
                    {
                        try
                        {
                            tmpDoubleList.Add(Convert.ToDouble(str1));
                        }
                        catch (FormatException)
                        {
                            tmpDoubleList.Add(0);
                        }
                    }
                    ret.Insert(0, new Tuple<List<double>, double>(tmpDoubleList, tempDouble));
                }
            }
            fs.BaseStream.Seek(0, SeekOrigin.Begin);
            return ret;
        }

        public List<Tuple<List<double>, double>> GetNormRhoByHeight(StreamReader dataFromFile)
        {
            var tmp = GetSpeedByHeight(dataFromFile);
            var maxRho = tmp.Max(it => it.Item1.Max());
            return
                tmp.ConvertAll(it => new Tuple<List<double>, double>(it.Item1.ConvertAll(inIt => inIt/maxRho), it.Item2));
        }
    }
}