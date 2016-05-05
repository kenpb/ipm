﻿using System.Management.Automation;
using System.Threading;

namespace ipm
{
    [System.Management.Automation.Cmdlet(VerbsCommon.New, "ISOFile")]
    public class DllMain : PSCmdlet
    {
        private ipmlib.ipm iso = new ipmlib.ipm();

        [Parameter(Position = 0, Mandatory = true)]
        public string path;
        
        private void Start()
        {
            iso.createISO(this.path);
        }

        protected override void ProcessRecord()
        {
            var myprogress = new ProgressRecord(1, "Creating", "Progress:");
            var isothread = new Thread(Start);
            isothread.Start();
            
            while (isothread.IsAlive)
            {
                myprogress.PercentComplete = iso.getProgress();
                WriteProgress(myprogress);
            }
            
            WriteObject("Done.");
        }
    }
}
