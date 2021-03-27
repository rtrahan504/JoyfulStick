using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.ComponentModel;
using System.Collections.ObjectModel;
using Microsoft.Win32;

namespace JoyfulStickGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public class TunedAxis
        {
            public string Name { get; set; }
            public double kP { get; set; }
            public double kD { get; set; }
            public double Resolution { get; set; }

            public double InputCenter { get; set; }
            public double InputMax { get; set; }
            public double InputMin { get; set; }

            public double OutputCenter { get; set; }
            public double OutputMax { get; set; }
            public double OutputMin { get; set; }

            public double PositiveExp { get { return m_PositiveExp; } set { m_PositiveExp = Math.Max(value, 0); } }
            public double NegativeExp { get { return m_NegativeExp; } set { m_NegativeExp = Math.Max(value, 0); } }

            [System.Xml.Serialization.XmlIgnore]
            public double OldInputValue = 0;
            [System.Xml.Serialization.XmlIgnore]
            public double Output = 0;
            [System.Xml.Serialization.XmlIgnore]
            public double OldErrors = 0;

            public TunedAxis() { }
            public TunedAxis(string Name, double kP, double kD, double Resolution)
            {
                this.Name = Name;
                this.kP = kP;
                this.kD = kD;
                this.Resolution = Resolution;

                this.InputCenter = 0;
                this.InputMax = 1;
                this.InputMin = -1;
                this.OutputCenter = 0;
                this.OutputMax = 1;
                this.OutputMin = -1;

                this.PositiveExp = 1.0;
                this.NegativeExp = 1.0;
            }

            [System.Xml.Serialization.XmlIgnore]
            private double m_PositiveExp = 1;
            [System.Xml.Serialization.XmlIgnore]
            private double m_NegativeExp = 1;
        }

        public class TunedButton
        {
            public int Index { get; set; }
            public double MaxPressTime { get; set; }

            public System.TimeSpan GetMaxPressTimeSpan() { return new System.TimeSpan(0, 0, 0, 0, (int)(MaxPressTime * 1000.0)); }

            [System.Xml.Serialization.XmlIgnore]
            public bool IsPressed = false;
            [System.Xml.Serialization.XmlIgnore]
            public System.DateTime TimePressed;

            public TunedButton() { }
            public TunedButton(int Index, double MaxPressTime_s)
            {
                this.Index = Index;
                this.MaxPressTime = 0;
            }
        }

        public class Profile : INotifyPropertyChanged
        {
            public String Name
            {
                get { return m_Name; }
                set
                {
                    m_Name = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("Name"));
                }
            }
            public List<TunedAxis> Axes { get; set; }
            public List<TunedButton> Buttons { get; set; }

            public Profile()
            { }
            public Profile(JoyfulStickWrapper.VJoyDevice vjoyDev)
            {
                this.Name = "{unnamed profile}";
                this.Axes = new List<TunedAxis>();
                this.Buttons = new List<TunedButton>();

                if (Axes.Count != 7)
                {
                    Axes.Clear();
                    Axes.Add(new TunedAxis("X", 0.5, 0.3, 1000));
                    Axes.Add(new TunedAxis("Y", 0.5, 0.3, 1000));
                    Axes.Add(new TunedAxis("Z", 0.2, 0.1, 1000));
                    Axes.Add(new TunedAxis("RX", 0.1, 0.3, 100));
                    Axes.Add(new TunedAxis("RY", 0.1, 0.3, 100));
                    Axes.Add(new TunedAxis("RZ", 0.5, 0.1, 100));
                    Axes.Add(new TunedAxis("Slider", 0.1, 0.1, 20));
                }

                var numbuttons = vjoyDev.GetNumButtons();
                if (Buttons.Count != numbuttons)
                {
                    Buttons.Clear();
                    for (int i = 0; i < numbuttons; ++i)
                        Buttons.Add(new TunedButton(i, 0));
                }
            }


            public event PropertyChangedEventHandler PropertyChanged;
 
            private String m_Name;

            public override String ToString() { return m_Name; }
        }

        public class JoyfulStickSettings
        { 
            public int CurrentGUIPage { get; set; }
            public int CurrentProfileIndex
            {
                get { return m_CurrentProfileIndex; } 
                set
                {
                    if (m_CurrentProfileIndex == value)
                        return;

                    m_CurrentProfileIndex = value;
                    Modified();

                    if (CurrentProfileChanged != null)
                        CurrentProfileChanged(this, CurrentProfile);
                }
            }

            public ObservableCollection<Profile> Profiles { get; private set; }
            [System.Xml.Serialization.XmlIgnore]
            public Profile CurrentProfile
            {
                get 
                { 
                    return CurrentProfileIndex >= 0 && CurrentProfileIndex < Profiles.Count ? Profiles[CurrentProfileIndex] : null;
                }
                set
                {
                    if (value is null)
                        CurrentProfileIndex = -1;
                    else
                    {
                        if (!Profiles.Contains(value))
                            Profiles.Add(value);
                        CurrentProfileIndex = Profiles.IndexOf(value);
                    }
                }
            }

            public event System.EventHandler<Profile> CurrentProfileChanged;

            public JoyfulStickSettings()
            {
                Profiles = new ObservableCollection<Profile>();
            }


            public static JoyfulStickSettings Load(String path)
            {
                JoyfulStickSettings ret = null;
                var serializer = new System.Xml.Serialization.XmlSerializer(typeof(JoyfulStickSettings));
                using (var stream = File.OpenRead(path))
                {
                    ret = serializer.Deserialize(stream) as JoyfulStickSettings;
                    ret.m_Modified = false;
                }
                return ret;
            }
            public void Save(String path)
            {
                TextWriter writer = new StreamWriter(path);
                var ser = new System.Xml.Serialization.XmlSerializer(typeof(JoyfulStickSettings));
                ser.Serialize(writer, this);
                writer.Close();

                m_Modified = false;
            }

            public void Modified() { m_Modified = true; }
            public bool IsModified() { return m_Modified; }

            int m_CurrentProfileIndex = -1;
            bool m_Modified = false;
        }

        DispatcherTimer timer = new DispatcherTimer(DispatcherPriority.ApplicationIdle);

        JoyfulStickWrapper.DirectInput dinput = null;
        JoyfulStickWrapper.Joystick p_dinputDev = null;
        JoyfulStickWrapper.Joystick dinputDev
        {
            get { return p_dinputDev; }
            set
            {
                this.p_dinputDev = value;

                m_InputDeviceGUI.RemoveAll();

                if (p_dinputDev != null)
                {
                    m_InputTab.Header = "Input: " + p_dinputDev.GetName();

                    m_InputDeviceGUI.AddAxis(0, "X", AxisBar.AxisAnchors.Center);
                    m_InputDeviceGUI.AddAxis(1, "Y", AxisBar.AxisAnchors.Center);
                    m_InputDeviceGUI.AddAxis(2, "Z", AxisBar.AxisAnchors.Left, true);
                    m_InputDeviceGUI.AddAxis(3, "RX", AxisBar.AxisAnchors.Center);
                    m_InputDeviceGUI.AddAxis(4, "RY", AxisBar.AxisAnchors.Center);
                    m_InputDeviceGUI.AddAxis(5, "RZ", AxisBar.AxisAnchors.Center);
                    m_InputDeviceGUI.AddAxis(6, "Slider", AxisBar.AxisAnchors.Left);

                    var numpovs = p_dinputDev.GetNumPOVs();
                    for (uint i = 0; i < numpovs; ++i)
                        m_InputDeviceGUI.AddPOV(i);

                    var numinputbuttons = p_dinputDev.GetNumButtons();
                    for (uint i = 0; i < numinputbuttons; ++i)
                        m_InputDeviceGUI.AddButton(i, p_dinputDev.GetButtonName(i));
                }
                else
                {
                    m_InputTab.Header = "Input: {unknown}";
                }
            }
        }

        JoyfulStickWrapper.VJoyDevice p_vJoyDev = null;
        JoyfulStickWrapper.VJoyDevice vjoyDev
        {
            get { return p_vJoyDev; }
            set
            {
                p_vJoyDev = value;

                m_OutputDeviceGUI.RemoveAll();

                if (p_vJoyDev != null)
                {
                    m_OutputTab.Header = "Output: vJoy";

                    m_OutputDeviceGUI.AddAxis(0, "X", AxisBar.AxisAnchors.Center);
                    m_OutputDeviceGUI.AddAxis(1, "Y", AxisBar.AxisAnchors.Center);
                    m_OutputDeviceGUI.AddAxis(2, "Z", AxisBar.AxisAnchors.Left, true);
                    m_OutputDeviceGUI.AddAxis(3, "RX", AxisBar.AxisAnchors.Center);
                    m_OutputDeviceGUI.AddAxis(4, "RY", AxisBar.AxisAnchors.Center);
                    m_OutputDeviceGUI.AddAxis(5, "RZ", AxisBar.AxisAnchors.Center);
                    m_OutputDeviceGUI.AddAxis(6, "Slider", AxisBar.AxisAnchors.Left);

                    var numpovs = p_vJoyDev.GetNumPOVs();
                    for (uint i = 0; i < numpovs; ++i)
                        m_OutputDeviceGUI.AddPOV(i);

                    var numModeButtons = 40;
                    uint iButton = 0;
                    for (uint mode = 0; mode < 3; ++mode)
                    {
                        m_OutputDeviceGUI.AddButtonPanel("Mode " + (mode + 1).ToString());
                        for (uint i = 0; i < numModeButtons; ++i)
                        {
                            m_OutputDeviceGUI.AddButton(iButton, iButton.ToString());
                            ++iButton;
                        }
                    }
                    if (iButton < p_vJoyDev.GetNumButtons())
                    {
                        m_OutputDeviceGUI.AddButtonPanel("Extra");
                        for (; iButton < p_vJoyDev.GetNumButtons(); ++iButton)
                        {
                            m_OutputDeviceGUI.AddButton(iButton, iButton.ToString());
                        }
                    }
                }
                else
                {
                    m_OutputTab.Header = "Output: {unknown}";
                }
            }
        }

        JoyfulStickWrapper.DirectOutput doutput = null;
        JoyfulStickWrapper.DirectOutputDevice doutputDev = null;

        JoyfulStickSettings p_Settings = null;
        JoyfulStickSettings Settings
        {
            get { return p_Settings; }
            set
            {
                if(p_Settings != null)
                    p_Settings.CurrentProfileChanged -= CurrentProfileChanged;

                p_Settings = null;

                if (value.CurrentGUIPage >= 0 && value.CurrentGUIPage < m_TabControl_Main.Items.Count)
                    m_TabControl_Main.SelectedIndex = value.CurrentGUIPage;

                value.CurrentProfileChanged += CurrentProfileChanged;

                if (value.Profiles.Count == 0)
                {
                    value.CurrentProfile = new Profile(vjoyDev);
                }

                m_ComboBox_Profile.ItemsSource = value.Profiles;
                m_ComboBox_Profile.SelectedItem = value.CurrentProfile;

                m_AxesGrid.ItemsSource = value.CurrentProfile.Axes;
                m_ButtonsGrid.ItemsSource = value.CurrentProfile.Buttons;

                p_Settings = value;

                CurrentProfileChanged(null, value.CurrentProfile);
            }
        }

        String m_SettingsFilename;

        public MainWindow()
        {
            InitializeComponent();

            dinput = new JoyfulStickWrapper.DirectInput();
            doutput = new JoyfulStickWrapper.DirectOutput();

            CheckDevices();

            // Setup the profiles
            try
            {
                m_SettingsFilename = "config.xml";
                Settings = JoyfulStickSettings.Load(m_SettingsFilename);
            }
            catch (System.Exception)
            {
                Settings = new JoyfulStickSettings();
            }

            timer.Interval = TimeSpan.FromSeconds(1.0 / 60.0);
            timer.Tick += timer_Tick;
            timer.Start();
        }

        void timer_Tick(object sender, EventArgs e)
        {
            CheckDevices();

            if (dinputDev != null && dinputDev.Poll())
            {
                if (vjoyDev != null && Settings.CurrentProfile != null)
                    UpdateControls();
            }
            else
            {
                if(dinputDev != null)
                    dinputDev.Dispose();
                dinputDev = null;

                if (doutputDev != null)
                    doutputDev.Dispose();
                doutputDev = null;
            }

            m_InputDeviceGUI.Update(dinputDev);
            m_OutputDeviceGUI.Update(vjoyDev);
        }

        JoyfulStickWrapper.DirectOutputDevice.Colors color = JoyfulStickWrapper.DirectOutputDevice.Colors.Off;
        bool ScrollingProfiles = false;

        void CheckDevices()
        {
            if (vjoyDev == null && JoyfulStickWrapper.VJoyDevice.GetNumDevices() > 0)
            {
                JoyfulStickWrapper.VJoyDevice device = null;
                try
                {
                    device = new JoyfulStickWrapper.VJoyDevice(1);
                }
                catch (Exception)
                { }

                vjoyDev = device;
            }

            if (dinput != null && dinputDev == null)
            {
                dinput.EnumerateJoysticks();
                if (dinput.GetNumJoysticks() >= 1)
                {
                    for (uint i = 0; i < dinput.GetNumJoysticks(); ++i)
                    {
                        try
                        {
                            var dev = dinput.GetJoystick(i);
                            if (dev.GetName() == "X52 Professional H.O.T.A.S.")
                            {
                                dinputDev = dev;
                                break;
                            }
                        }
                        catch (Exception)
                        { }
                    }
                }
            }

            if (doutput != null && doutputDev == null)
            {
                doutput.EnumerateDevices();
                if (doutput.GetNumDevices() >= 1)
                {
                    doutputDev = doutput.GetDevice(0);
                    doutputDev.AddPage(1);

                    if (Settings != null)
                        UpdateProfileDisplay(Settings.CurrentProfile);
                }
            }
        }

        void UpdateControls()
        {
            var now = System.DateTime.Now;
            var numButtons = dinputDev.GetNumButtons();

            // Change modes with a joystick key combination
            if (numButtons >= 35 && dinputDev.GetButtonValue(30) && (dinputDev.GetButtonValue(34) || dinputDev.GetButtonValue(35)) && !ScrollingProfiles)
            {
                var index = Settings.CurrentProfileIndex;

                if (dinputDev.GetButtonValue(35))
                    index++;
                else if (dinputDev.GetButtonValue(34))
                    index--;

                if (index >= Settings.Profiles.Count)
                    index = 0;
                else if (index < 0)
                    index = Settings.Profiles.Count - 1;

                Settings.CurrentProfileIndex = index;
                m_ComboBox_Profile.SelectedItem = Settings.CurrentProfile;
                ScrollingProfiles = true;
            }
            else
                ScrollingProfiles = false;

            // Update the axes
            {
                var Axes = Settings.CurrentProfile.Axes;
                var numAxes = Math.Min(vjoyDev.GetNumAxes(), Math.Min(dinputDev.GetNumAxes(), Axes.Count));

                for (int i = 0; i < numAxes; ++i)
                {
                    var input = dinputDev.GetAxisValue((uint)i);

                    // Reduce resolution of the axis
                    var res = Axes[i].Resolution - 1;
                    if (Math.Abs(input - Axes[i].OldInputValue) < 2.0 / res)
                        input = Axes[i].OldInputValue;
                    else
                        Axes[i].OldInputValue = Math.Round(input * res) / res;

                    // Account for the input tuning
                    double correctedInput = 0;
                    if (input > Axes[i].InputCenter)
                        correctedInput = (Math.Min(input, Axes[i].InputMax) - Axes[i].InputCenter) / (Axes[i].InputMax - Axes[i].InputCenter);
                    if (input < Axes[i].InputCenter)
                        correctedInput = (Math.Max(input, Axes[i].InputMin) - Axes[i].InputCenter) / (Axes[i].InputCenter - Axes[i].InputMin);

                    // Compute commanded position based on endpoints, centerpoint, and exponential.
                    double commandedPosition = Axes[i].OutputCenter;
                    if (correctedInput > 0)
                        commandedPosition += Math.Min(1, Math.Pow(correctedInput, Axes[i].PositiveExp)) * (Axes[i].OutputMax - Axes[i].OutputCenter);
                    else if (correctedInput < 0)
                        commandedPosition -= Math.Min(1, Math.Pow(-correctedInput, Axes[i].NegativeExp)) * (Axes[i].OutputCenter - Axes[i].OutputMin);

                    if(i == 2)
                    {
                        System.Console.WriteLine(input.ToString() + " " + commandedPosition.ToString());
                    }

                    // Smooth the motion  with PD
                    double error = commandedPosition - Axes[i].Output;
                    double derror = error - Axes[i].OldErrors;
                    Axes[i].OldErrors = error;
                    Axes[i].Output = Axes[i].Output + error * Axes[i].kP - derror * Axes[i].kD;

                    vjoyDev.SetAxisValue(Axes[i].Output, (uint)i);
                }
            }

            vjoyDev.SetPOVValue(dinputDev.GetPOVValue(0), 0);

            {
                uint Mode;
                if (dinputDev.GetButtonValue(27)) Mode = 0;
                else if (dinputDev.GetButtonValue(28)) Mode = 1;
                else if (dinputDev.GetButtonValue(29)) Mode = 2;
                else Mode = 0;

                for (uint iMode = 0; iMode < 3; ++iMode)
                {
                    for (uint iInputButton = 0; iInputButton < numButtons; ++iInputButton)
                    {
                        uint iOutputButton = iInputButton + 40 * iMode;
                        bool pressed = iMode == Mode && dinputDev.GetButtonValue(iInputButton);

                        TunedButton button = Settings.CurrentProfile.Buttons[(int)iOutputButton];

                        if (pressed)
                        {
                            if (!button.IsPressed)
                            {
                                button.TimePressed = now;
                                button.IsPressed = true;
                            }
                            else if (button.MaxPressTime > 0 && now - button.TimePressed > button.GetMaxPressTimeSpan())
                                pressed = false;
                        }
                        else
                            button.IsPressed = false;

                        vjoyDev.SetButtonValue(pressed, iOutputButton);
                    }
                }

                JoyfulStickWrapper.DirectOutputDevice.Colors newColor = JoyfulStickWrapper.DirectOutputDevice.Colors.Off;
                if (Mode == 0)
                    newColor = JoyfulStickWrapper.DirectOutputDevice.Colors.Red;
                else if (Mode == 1)
                    newColor = JoyfulStickWrapper.DirectOutputDevice.Colors.Amber;
                else if (Mode == 2)
                    newColor = JoyfulStickWrapper.DirectOutputDevice.Colors.Green;

                if (newColor != color && doutputDev != null)
                {
                    color = newColor;
                    bool res = true;
                    for (uint i = 0; i < (uint)JoyfulStickWrapper.DirectOutputDevice.LEDs.Count; ++i)
                        res = doutputDev.SetLED(1, (JoyfulStickWrapper.DirectOutputDevice.LEDs)i, color);
                }
            }

            try
            {
                vjoyDev.CommitChanges();
            }
            catch(Exception)
            {
                vjoyDev = null;
            }
        }
        
        void UpdateProfileDisplay(Profile profile)
        {
            if (doutputDev != null)
            {
                doutputDev.SetText(1, JoyfulStickWrapper.DirectOutputDevice.MFD_Line._0, "");
                doutputDev.SetText(1, JoyfulStickWrapper.DirectOutputDevice.MFD_Line._2, "");

                if (profile is null)
                    doutputDev.SetText(1, JoyfulStickWrapper.DirectOutputDevice.MFD_Line._1, "");
                else
                    doutputDev.SetText(1, JoyfulStickWrapper.DirectOutputDevice.MFD_Line._1, profile.Name);
            }
        }

        void CurrentProfileChanged(object sender, Profile profile)
        {
            UpdateProfileDisplay(profile);

            if (profile is null)
            {
                m_AxesGrid.ItemsSource = null;
                m_ButtonsGrid.ItemsSource = null;
            }
            else
            {
                m_AxesGrid.ItemsSource = profile.Axes;
                m_ButtonsGrid.ItemsSource = profile.Buttons;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (Settings.IsModified())
            {
                var res = System.Windows.MessageBox.Show("Do you wish to save your previous changes?", "Confirm", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                if (res == MessageBoxResult.Yes)
                    Save(m_SettingsFilename);
                else if (res == MessageBoxResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }
            }

            timer.Stop();

            if (dinputDev != null) dinputDev.Dispose(); dinputDev = null;
            if (dinput != null) dinput.Dispose(); dinput = null;
            if (doutputDev != null) doutputDev.Dispose(); doutputDev = null;
            if (doutput != null) doutput.Dispose(); doutput = null;
            if (vjoyDev != null) vjoyDev.Dispose(); vjoyDev = null;
        }

        private void m_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            Settings.Modified();
        }

        private void m_Button_DeleteProfile_Click(object sender, RoutedEventArgs e)
        {
            Settings.Modified();

            Settings.Profiles.Remove(Settings.CurrentProfile);
            --Settings.CurrentProfileIndex;
            m_ComboBox_Profile.SelectedItem = Settings.CurrentProfile;
        }

        private void m_Button_AddProfile_Click(object sender, RoutedEventArgs e)
        {
            Settings.Modified();

            Settings.CurrentProfile = new Profile(vjoyDev);
            m_ComboBox_Profile.SelectedItem = Settings.CurrentProfile;
        }

        private void m_ComboBox_Profile_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Settings == null)
                return;

            var current = Settings.CurrentProfile;
            if (current is null)
                return;

            if (current.Name != m_ComboBox_Profile.Text)
            {
                Settings.Modified();
                current.Name = m_ComboBox_Profile.Text;
            }
        }

        private void m_ComboBox_Profile_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (Settings != null)
            {
                Settings.Modified();

                if (e.AddedItems.Count == 1)
                    Settings.CurrentProfile = e.AddedItems[0] as Profile;
            }
        }

        private void Save(String filename)
        {
            if(filename.Length == 0)
            {
                var dlg = new SaveFileDialog();
                dlg.DefaultExt = ".xml";
                dlg.Filter = "XML file (.xml)|*.xml";
                dlg.ValidateNames = true;
                var res = dlg.ShowDialog(this);
                if (!res.HasValue || !res.Value)
                    return;
                m_SettingsFilename = dlg.FileName;
            }

            Settings.CurrentGUIPage = m_TabControl_Main.SelectedIndex;
            Settings.Save(m_SettingsFilename);
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem item = sender as MenuItem;

            var Text = item.Header.ToString();

            if (Text == "_New")
            {
                if (Settings.IsModified())
                {
                    var res = System.Windows.MessageBox.Show("Do you wish to save your previous changes?", "Confirm", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                    if (res == MessageBoxResult.Yes)
                        Save(m_SettingsFilename);
                    else if (res == MessageBoxResult.Cancel)
                        return;
                }

                Settings = new JoyfulStickSettings();
            }
            else if (Text == "_Load")
            {
                if (Settings.IsModified())
                {
                    var res = System.Windows.MessageBox.Show("Do you wish to save your previous changes?", "Confirm", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                    if (res == MessageBoxResult.Yes)
                        Save(m_SettingsFilename);
                    else if (res == MessageBoxResult.Cancel)
                        return;
                }

                {
                    var dlg = new OpenFileDialog();
                    dlg.DefaultExt = ".xml";
                    dlg.Filter = "XML file (.xml)|*.xml";
                    var res = dlg.ShowDialog();
                    if (res.HasValue && res.Value)
                    {
                        try
                        {
                            Settings = JoyfulStickSettings.Load(dlg.FileName);
                        }
                        catch (System.Exception)
                        {
                            Settings = new JoyfulStickSettings();
                        }
                    }
                }
            }
            else if (Text == "_Save")
            {
                Save(m_SettingsFilename);
            }
            else if (Text == "_Save As")
            {
                Save("");
            }
            else if(Text == "_Exit")
            {
                this.Close();
            }

            else if (Text == "_Reconnect Input Device")
            {
                if (dinputDev != null)
                    dinputDev.Dispose();
                dinputDev = null;

                if (doutputDev != null)
                    doutputDev.Dispose();
                doutputDev = null;
            }

            else if (Text == "_About")
            {
                var about = new About();
                about.ShowDialog();
            }
        }
    }
}
