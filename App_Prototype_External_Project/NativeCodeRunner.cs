using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using UnityEngine;

public class NativeCodeRunner : MonoBehaviour
{
    // Start is called before the first frame update
    AndroidJavaObject bluet;
    AndroidJavaObject alert;
    AndroidJavaClass unityPlayerClass;
    AndroidJavaObject unityActivity;
    public Double prom = 0;
    public float posY = 0;
    public int posGL = 0;

    void Start()
    {
        CallNativePlugin();
    }

    //method that calls our native plugin.
    public void CallNativePlugin()
    {
        // Retrieve the UnityPlayer class.
        unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        // Retrieve the UnityPlayerActivity object ( a.k.a. the current context )
        unityActivity = unityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");
        // Retrieve the "Bridge" from our native plugin.
        // ! Notice we define the complete package name.              
        //alert = new AndroidJavaObject("com.millerbsv.bluetoothhc.Alert");
        bluet = new AndroidJavaObject("com.millerbsv.bluetoothhc.Bluetooth");

        //bluet = new AndroidJavaClass("com.millerbsv.bluetoothhc.Bluetooth");

        // Setup the parameters we want to send to our native plugin.              
        //object[] parameters = new object[2];
        //parameters[0] = unityActivity;
        //parameters[1] = "Hello World!";
        //alert.Call("PrintString", parameters);

        // Call PrintString in bridge, with our parameters.
        string data = bluet.Call<string>("getDevices");
        //parameters[1] = data;

        posY = GameData.GetAbsolutPositionFromMl((float)prom) + 0.78f;

        posGL = GameData.GetMlPositionFromAbsolut(posY);

        if (data != null)
        {
            string[] subs = data.Split(';');
            Debug.LogWarningFormat("Devices: {0}", data);
            //Debug.LogWarningFormat("Device 1: {0}", subs[1]);


            //alert.Call("PrintString", parameters);
            object[] parameters2 = new object[2];
            parameters2[0] = 4;
            parameters2[1] = "00:20:12:08:B3:43";
            bluet.Call("connectToDevice", parameters2);

            InvokeRepeating("OutputTime", 2f, 0.2f);  //1s delay, repeat every 1s

        }





    }

    void OutputTime()
    {
        //Debug.LogWarning("Obtiene la data");
        string data = bluet.Call<string>("getData");
        //string data = "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.27" + Environment.NewLine + "0.";
        object[] parameters = new object[2];
        //Debug.LogWarningFormat("dataBluetooth {0}", data);
        parameters[0] = unityActivity;
        string[] strings = Regex.Split(data, Environment.NewLine);
        List<double> valuesList = new List<double>();
        Double tempValue;
        for (int i = 1; i < strings.Length-1; i++)
        {
            if(Double.TryParse(strings[i], out tempValue))
            {
                valuesList.Add(tempValue);
            } else
            {
                Console.WriteLine("error parse data {0}", strings[i]);
            }
        }
        Double[] valuesDouble = valuesList.ToArray();
        if(valuesDouble.Length > 0)
        {
            //Debug.LogWarningFormat("valuesDouble {0}", valuesDouble.ToString());
            prom = valuesDouble.Average() * 1000;
            Debug.LogWarningFormat("promedio: {0}", prom.ToString());
            posY = GameData.GetAbsolutPositionFromMl((float)prom) + 0.78f;
            Debug.LogWarningFormat("position Y: {0}", posY.ToString());
            posGL = GameData.GetMlPositionFromAbsolut(posY);
            Debug.LogWarning("values : " + String.Join(",", strings));
            //parameters[1] = prom.ToString();
            //Debug.LogWarning(prom.ToString());

            //alert.Call("PrintString", parameters);
        }
    }
}
