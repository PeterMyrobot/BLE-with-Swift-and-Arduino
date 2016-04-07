//
//  ViewController.swift
//  LedStripDemo
//
//  Created by apple on 01/04/2016.
//  Copyright © 2016 Peter. All rights reserved.
//

import UIKit
import CoreBluetooth

class ViewController: UIViewController, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    var strips:[ledStrip]=[ledStrip(),ledStrip(),ledStrip(),ledStrip(),ledStrip(),ledStrip()]
    
    @IBOutlet var backgroundImageView: UIImageView!
    
    @IBOutlet weak var redValueLabel: UILabel!
    @IBOutlet weak var greenValueLabel: UILabel!
    @IBOutlet weak var blueValueLabel: UILabel!
    
    @IBOutlet weak var redSilder: UISlider!
    @IBOutlet weak var greenSlider: UISlider!
    @IBOutlet weak var blueSlider: UISlider!
    
    @IBOutlet weak var ColorButton: UIButton!

    @IBOutlet weak var textField: UITextField!
    
    @IBAction func colorButton(sender: AnyObject) {
        ColorButton.backgroundColor = UIColor(colorLiteralRed: redSilder.value/255, green: greenSlider.value/255, blue: blueSlider.value/255, alpha: 1.0)
        ColorButton.tintColor = UIColor(colorLiteralRed: redSilder.value/255, green: greenSlider.value/255, blue: blueSlider.value/255, alpha: 1.0)
        
        textField.text = "\(Int(redSilder.value)),\(Int(greenSlider.value)),\(Int(blueSlider.value)),"
        
        writeout()
        
        textField.text = "\(Int(strips[0].isLight)),\(Int(strips[1].isLight)),\(Int(strips[2].isLight)),\(Int(strips[3].isLight)),\(Int(strips[4].isLight)),\(Int(strips[5].isLight)),"
         writeout()

        
    }
    @IBAction func bulbChosoe(sender: UIButton) {
        let number = NSNumberFormatter().numberFromString(sender.currentTitle!)!.integerValue
        
        if !strips[number].isLight{
            sender.setImage(UIImage(named: "lightBulb"), forState: UIControlState.Normal)
            strips[number].isLight = true
        }else {
            sender.setImage(UIImage(named: "darkBulb"), forState: UIControlState.Normal)
            strips[number].isLight = false
        }
    }
    
    //Silder change value
    @IBAction func redValueChange(sender: AnyObject) {
        redValueLabel.text = "Red: \(Int(redSilder.value))"
        greenValueLabel.text = "Green: \(Int(greenSlider.value))"
        blueValueLabel.text = "blue: \(Int(blueSlider.value))"
    }

    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // 背景模糊
        let blurEffect = UIBlurEffect(style: UIBlurEffectStyle.Dark)
        let blurEffectView = UIVisualEffectView(effect: blurEffect)
        blurEffectView.frame = view.bounds
        backgroundImageView.addSubview(blurEffectView)
        
        //圓形按鈕
        ColorButton.backgroundColor = UIColor.whiteColor()
        ColorButton.tintColor = UIColor.whiteColor()
        ColorButton.layer.cornerRadius = self.ColorButton.frame.width/2
        
        manager  = CBCentralManager(delegate: self, queue: nil)

        
        
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    //BLE
    var manager: CBCentralManager!
    var Arduino: CBPeripheral!
    var writeValue: CBCharacteristic?
    var readValue: CBCharacteristic?
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        if(peripheral.name == "UART"){
            
            self.Arduino = peripheral
            self.Arduino.delegate = self
            manager.stopScan()
            manager.connectPeripheral(Arduino, options: nil)

        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if let servicePeripherals = peripheral.services as [CBService]!{
            for service in servicePeripherals{
                peripheral.discoverCharacteristics(nil, forService: service)
            }
        }
    }
    
    func  peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if let characterArray = service.characteristics as [CBCharacteristic]!{
            for cc in characterArray{
                print(cc)
                
                if(cc.UUID.UUIDString == "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"){
                    print("UART Characteristic UUID found !")
                }
                if(cc.UUID.UUIDString == "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"){
                    print ("RX Characteristic UUID found !")
                    self.readValue = (cc)
                    peripheral.setNotifyValue(true, forCharacteristic: cc )
                }
                if(cc.UUID.UUIDString == "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"){
                    self.writeValue = (cc)
                    print("TX Characteristic UUID found !")

                }
                
            }
        }
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices(nil)
    }
    
    func centralManagerDidUpdateState(central: CBCentralManager) {
        switch (central.state){
        case.PoweredOff:
            print( "Device powered off")
        case.PoweredOn:
            print( "Device Ready")
            manager.scanForPeripheralsWithServices(nil, options: nil )
        case.Resetting:
            print( "Device is resetting")
        case.Unauthorized:
            print( "Device not authorized")
        case.Unknown:
            print( "BLE state unknow")
        case.Unsupported:
            print("Device not supported")
        }
    }
    
    func writeout() {
        if let writeValue = self.writeValue{

            let singleCharString = textField.text!
            let Value: NSData = singleCharString.dataUsingEncoding(NSUTF8StringEncoding)!

            self.Arduino.writeValue(Value, forCharacteristic: writeValue, type: CBCharacteristicWriteType.WithoutResponse)
        }
    }


}

