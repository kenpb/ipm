using System.Management.Automation;
using System.Threading;
using System;

namespace ipm
{
    [System.Management.Automation.Cmdlet(VerbsCommon.New, "ISOFile")]
    public class DllMain : PSCmdlet
    {
        private ipmlib.ipm iso = new ipmlib.ipm();

        [Parameter(Position = 0, Mandatory = true)]
        public string path;
        [Parameter(Position = 1, Mandatory = false)]
        public string outpath;
        
        private void Start(string path, string outpath)
        {
            iso.CreateISO(path, outpath);
        }

        private int GetProgress()
        {
            return iso.GetProgress();
        }

        protected override void ProcessRecord()
        {
            var myprogress = new ProgressRecord(1, "Creating: " + this.path + ".iso", "Progress:");
            var isothread = new Thread(() => Start(this.path, this.outpath));
            isothread.Start();

            Console.WriteLine("Reading Target directory...");
            
            while (isothread.IsAlive)
            {
                myprogress.PercentComplete = GetProgress();
                Thread.Sleep(200);
                if (myprogress.PercentComplete != 0)
                {
                    WriteProgress(myprogress);
                }
            }
            
            WriteObject("Done...");
            Console.Beep();
        }
    }
}
