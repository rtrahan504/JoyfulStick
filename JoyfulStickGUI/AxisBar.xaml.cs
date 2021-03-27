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
    /// Interaction logic for AxisBar.xaml
    /// </summary>
    public partial class AxisBar : UserControl
    {
        public enum AxisAnchors
        {
            Center, Left
        }

        public AxisAnchors AxisAnchor { get; set; }

        public bool Reverse { get; set; }

        public AxisBar()
        {
            InitializeComponent();
        }

        public double Value
        {
            get { return m_Value; }
            set
            {
                m_Value =
                    value > 1.0 ? 1.0 :
                    value < -1.0 ? -1.0 :
                    value;

                var dispValue = Reverse ? -m_Value : m_Value;

                if (AxisAnchor == AxisAnchors.Center)
                {
                    if (dispValue > 0)
                    {
                        m_Grid.ColumnDefinitions[0].Width = new GridLength(100, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[1].Width = new GridLength(100 * dispValue, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[2].Width = new GridLength(100 - dispValue * 100, GridUnitType.Star);
                    }
                    else if (dispValue < 0)
                    {
                        m_Grid.ColumnDefinitions[0].Width = new GridLength(100 + dispValue * 100, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[1].Width = new GridLength(-100 * dispValue, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[2].Width = new GridLength(100, GridUnitType.Star);
                    }
                    else
                    {
                        m_Grid.ColumnDefinitions[0].Width = new GridLength(100, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[1].Width = new GridLength(0, GridUnitType.Star);
                        m_Grid.ColumnDefinitions[2].Width = new GridLength(100, GridUnitType.Star);
                    }
                }
                if (AxisAnchor == AxisAnchors.Left)
                {
                    dispValue = (dispValue + 1.0) * 0.5;

                    m_Grid.ColumnDefinitions[0].Width = new GridLength(0, GridUnitType.Star);
                    m_Grid.ColumnDefinitions[1].Width = new GridLength(dispValue * 100, GridUnitType.Star);
                    m_Grid.ColumnDefinitions[2].Width = new GridLength(100 - dispValue * 100, GridUnitType.Star);
                }

                m_ValueLabel.Content = Math.Round(m_Value * 100.0).ToString() + "%";
            }
        }

        private double m_Value = 0;
    }
}
