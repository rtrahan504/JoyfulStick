using System;
using System.Collections.Generic;
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

namespace JoyfulStickGUI
{
    /// <summary>
    /// Interaction logic for DeviceGUI.xaml
    /// </summary>
    public partial class DeviceGUI : UserControl
    {
        public DeviceGUI()
        {
            InitializeComponent();
        }

        public void RemoveAll()
        {
            m_DeviceGrid.Children.Clear();
            m_DeviceGrid.RowDefinitions.Clear();

            m_Axes.Clear();
            m_POVs.Clear();
            m_InputButtons.Clear();

            m_POVPanel = null;
            m_ButtonPanel = null;
        }

        public void AddAxis(uint i, string Name, AxisBar.AxisAnchors anchor, bool Reverse = false)
        {
            var newRow = new RowDefinition();
            m_DeviceGrid.RowDefinitions.Add(newRow);
            newRow.Height = new GridLength(0, GridUnitType.Auto);

            var label = new Label();
            label.Content = Name + ":";
            label.Padding = new Thickness(0, 0, 5, 0);
            m_DeviceGrid.Children.Add(label);
            Grid.SetRow(label, m_DeviceGrid.RowDefinitions.Count - 1);
            Grid.SetColumn(label, 0);

            AxisBar newAxis = new AxisBar();
            newAxis.AxisAnchor = anchor;
            newAxis.BorderBrush = SystemColors.ActiveBorderBrush;
            newAxis.BorderThickness = new Thickness(1);
            newAxis.Reverse = Reverse;
            m_DeviceGrid.Children.Add(newAxis);
            Grid.SetRow(newAxis, m_DeviceGrid.RowDefinitions.Count - 1);
            Grid.SetColumn(newAxis, 1);
            m_Axes[i] = newAxis;
        }
        public void SetAxis(uint i, double value)
        {
            m_Axes[i].Value = value;
        }

        public void AddPOV(uint i)
        {
            if (m_POVPanel == null)
            {
                var newRow = new RowDefinition();
                m_DeviceGrid.RowDefinitions.Add(newRow);
                newRow.Height = new GridLength(0, GridUnitType.Auto);

                var label = new Label();
                label.Content = "POVs:";
                label.Padding = new Thickness(0, 0, 5, 0);
                m_DeviceGrid.Children.Add(label);
                Grid.SetRow(label, m_DeviceGrid.RowDefinitions.Count - 1);
                Grid.SetColumn(label, 0);

                m_POVPanel = new WrapPanel();
                m_DeviceGrid.Children.Add(m_POVPanel);
                Grid.SetRow(m_POVPanel, m_DeviceGrid.RowDefinitions.Count - 1);
                Grid.SetColumn(m_POVPanel, 1);
            }

            POV newPOV = new POV();
            newPOV.DisplayName = i.ToString();
            m_POVPanel.Children.Add(newPOV);
            m_POVs[i] = newPOV;
        }
        public void SetPOV(uint i, double angle)
        {
            m_POVs[i].Value = angle;
        }

        public void AddButtonPanel(string name)
        {
            var newRow = new RowDefinition();
            m_DeviceGrid.RowDefinitions.Add(newRow);
            newRow.Height = new GridLength(0, GridUnitType.Auto);

            var label = new Label();
            label.Content = name + ":";
            label.Padding = new Thickness(0, 0, 5, 0);
            m_DeviceGrid.Children.Add(label);
            Grid.SetRow(label, m_DeviceGrid.RowDefinitions.Count - 1);
            Grid.SetColumn(label, 0);

            m_ButtonPanel = new WrapPanel();
            m_DeviceGrid.Children.Add(m_ButtonPanel);
            Grid.SetRow(m_ButtonPanel, m_DeviceGrid.RowDefinitions.Count - 1);
            Grid.SetColumn(m_ButtonPanel, 1);
        }
        public void AddButton(uint i, string name)
        {
            if (m_ButtonPanel == null)
                AddButtonPanel("Buttons");

            name = name.Replace("Button ", "");

            Label newLabel = new Label();
            newLabel.Content = name;
            newLabel.BorderBrush = SystemColors.ActiveBorderBrush;
            newLabel.BorderThickness = new Thickness(1);
            newLabel.HorizontalContentAlignment = HorizontalAlignment.Center;
            newLabel.MinWidth = 24;
            newLabel.Padding = new Thickness(0, 1, 0, 1);
            m_ButtonPanel.Children.Add(newLabel);
            m_InputButtons[i] = newLabel;
        }
        public void SetButton(uint i, bool pressed)
        {
            m_InputButtons[i].Background = pressed ? new SolidColorBrush(Color.FromRgb(0, 0, 175)) : new SolidColorBrush(Color.FromRgb(34, 34, 34));
        }

        private WrapPanel m_POVPanel;
        private WrapPanel m_ButtonPanel;

        private Dictionary<uint, AxisBar> m_Axes = new Dictionary<uint, AxisBar>();
        private Dictionary<uint, POV> m_POVs = new Dictionary<uint, POV>();
        private Dictionary<uint, Label> m_InputButtons = new Dictionary<uint, Label>();

        public void Update(JoyfulStickWrapper.Joystick device)
        {
            if (device is null)
            {
                RemoveAll();
                return;
            }

            for (uint i = 0; i < device.GetNumAxes(); ++i)
                SetAxis(i, device.GetAxisValue(i));

            for (uint i = 0; i < device.GetNumPOVs(); ++i)
                SetPOV(i, device.GetPOVValue(i));

            for (uint i = 0; i < device.GetNumButtons(); ++i)
                SetButton(i, device.GetButtonValue(i));
        }
        public void Update(JoyfulStickWrapper.VJoyDevice device)
        {
            if (device is null)
            {
                RemoveAll();
                return;
            }

            for (uint i = 0; i < device.GetNumAxes(); ++i)
                SetAxis(i, device.GetAxisValue(i));

            for (uint i = 0; i < device.GetNumPOVs(); ++i)
                SetPOV(i, device.GetPOVValue(i));

            for (uint i = 0; i < device.GetNumButtons(); ++i)
                SetButton(i, device.GetButtonValue(i));
        }
    }
}
