import quandl 
import datetime
import pandas as pd 
import numpy as np
import math as math
import matplotlib.pyplot as plt
from sklearn.model_selection import GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import SGDRegressor
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
from sklearn.svm import SVR
from pandas.plotting import register_matplotlib_converters
from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()
plt.ion()

#given a stock id,start date,end date,will return data from start-end.
def load_data(stock,start_date,end_date):
        data = quandl.get(stock,start_date = start_date,end_date = end_date, authtoken="5MUfamxGXvdCRjZyQ5jm")
        return data

#returns a new dataframe containing the features taken from original dataframe
def features(df):
        data = pd.DataFrame()
        data['open'] = df['Open'] #opening price of stock
        data['close'] = df['Close']#closing price of stock
        data['open_1'] = df['Open'].shift(1) #the previous day's opening stock price
        data['close_1'] = df['Close'].shift(1) #the previous day's closing stock price
        data['high_1'] = df['High'].shift(1) #the previous day's high stock price
        data['low_1'] = df['Low'].shift(1) #the previous day's low stock price 
        data['volume_1'] = df['Volume'].shift(1) #the previous day's total amount of stock traded  
        #return values:
        data['daily_return'] = ((df['Close'] - df['Close'].shift(1)) /df['Close'].shift(1)).shift(1)
        data['weekly_return'] = ((df['Close'] - df['Close'].shift(5))/df['Close'].shift(5)).shift(1)
        data['monthly_return'] = ((df['Close'] - df['Close'].shift(21))/df['Close'].shift(21)).shift(1)
        data['yearly_return'] = ((df['Close'] - df['Close'].shift(252))/df['Close'].shift(252)).shift(1)   
        #weekly = 5,montlhy = 21,yearly = 252 trading days/windows
        #moving avg of weekly,monthy,yearly features:
        #closing price avgs:
        data['avg_close_week'] =df['Close'].shift(1).rolling(window = 5).mean()
        data['avg_close_month'] = df['Close'].shift(1).rolling(window=21).mean()
        data['avg_close_year'] = df['Close'].shift(1).rolling(window = 252).mean()
         #volume price avgs:
        data['avg_volume_week'] = df['Volume'].shift(1).rolling(window=5).mean()
        data['avg_volume_month'] = df['Volume'].shift(1).rolling(window=21).mean()
        data['avg_volume_year'] = df['Volume'].shift(1).rolling(window=252).mean()
        #standard deviation(std) of feaures:
        #closing standard deviation:
        data['avg_std_close_week'] =df['Close'].shift(1).rolling(window=5).std()
        data['avg_std_close_month'] = df['Close'].shift(1).rolling(window=21).std()
        data['avg_std_close_year']  = df['Close'].shift(1).rolling(window=252).std()
        #volume standard deviation:
        data['avg_std_volume_week'] = df['Volume'].shift(1).rolling(window=5).std()
        data['avg_std_volume_month'] = df['Volume'].shift(1).rolling(window=21).std()
        data["avg_std_volume_year"] = df['Volume'].shift(1).rolling(window=252).std()            
         #features based on ratios of weekly,month,yearly avgs:
         #closing avg ratios: 
        data['avg_week_month_close_ratio'] = data['avg_close_week']/data['avg_close_month']
        data['avg_week_year_close_ratio'] = data['avg_close_week']/data['avg_close_year']
        data['avg_month_year_close_ration'] = data['avg_close_month']/data['avg_close_year']
       #volume avg ratio:
        data['avg_week_month_volume_ratio'] = data['avg_volume_week']/data['avg_volume_month']
        data['avg_week_year_volume_ratio'] = data['avg_volume_week']/data['avg_volume_year']
        data['avg_month_year_volume_ratio'] = data['avg_volume_month']/data['avg_volume_year'] 
         #standard deviation close ratios:
        data['avg_week_month_std_close_ratio'] = data['avg_std_close_week'] / data['avg_std_close_month']
        data['avg_week_year_std_close_ratio'] = data['avg_std_close_week']/data['avg_std_close_year']
        data['avg_month_year_std_close_ratio'] = data['avg_std_close_month']/data['avg_std_close_year']
       #closing std ratios:
        data['avg_week_month_std_volume_ratio'] = data['avg_std_volume_week']/data['avg_std_volume_month']
        data['avg_week_year_std_volume_ratio'] = data['avg_std_volume_week']/data['avg_std_volume_year']
        data['avg_month_year_std_volume_ratio'] = data['avg_std_volume_month']/data['avg_std_volume_year']
      #return avgs:
        data['avg_return_week'] = data['daily_return'].rolling(window = 5).mean()
        data['avg_return month'] = data['daily_return'].rolling(window = 21).mean()
        data['avg_return_year'] = data['daily_return'].rolling(window = 252).mean()
        #volatility 
        #data['volatility_daily'] = math.log10( (data['close'] - data['close_1']) / data['close'])
        # data['volatility_weekly'] = data['volatility'].shift(1).rolling(window = 5).std()
        # data['volatility_monthly'] = data['volatility'].shift(1).rolling(window = 21).std()
        # data['volatility_yearly'] = data['volatility'].shift(1).rolling(window = 252).std()
        # data['annualized_volatility_yearly'] = data['volatility_yearly'] * math.sqrt(252)
        data = data.dropna(axis=0)
        return data

def set_stocks(stock_names,start,end):
        normalized_stock_data = {}
        for k,v in stock_names.items():
                print('loading in:' + k)
                data = pd.DataFrame()
                data = load_data(stock_names[k],start,end)
                data = features(data)
                normalized_stock_data[k]  = data
        return normalized_stock_data

def print_stocks(data):
        for k,v in data.items():
                print (data[k])

def training_sets(stocks,start,end):
        data = {}
        for k,v in stocks.items():
                data[k] = v.loc[start:end]
        return data

def test_sets(stocks,start,end):
        data = {}
        for k,v in stocks.items():
                data[k] = v.loc[start:end]
        return data

def predictions(stocks,training,test):
        scaler = StandardScaler()
        y_column = 'close'
        param_grid = {
        "alpha": [1e-5, 3e-5, 1e-4],
        "eta0": [0.01, 0.03, 0.1],
         }
        data= {}
        for k,v in stocks.items():
                linear_regression = SGDRegressor(max_iter= 100000)
                grid_search = GridSearchCV(linear_regression, param_grid, cv=5,iid ='false')
                X_columns = v.drop(['close'],axis = 1).columns
                X_train = (training[k])[X_columns]
                y_train = (training[k])[y_column]
                x_test = (test[k])[X_columns]
                scaler.fit(X_train)
                X_scaled_train = scaler.transform(X_train)
                X_scaled_test = scaler.transform(x_test)
                grid_search.fit(X_scaled_train, y_train)
                lr_best = grid_search.best_estimator_

                data[k] = lr_best.predict(X_scaled_test)
        return data             

def graph(predictions,test):
        fig, ax = plt.subplots(nrows= 5,ncols= 4,sharex= True)
        for a,key in zip(ax.flat,predictions.keys()):
                dates = test[key].index.values
                test_plot = (test[key])['close'].to_numpy()
                lr_plot = predictions[key]
                a.plot(dates,test_plot,'k')
                a.plot(dates,lr_plot,'r')
                a.set_title(key)
                a.grid(True)
                plt.draw()

def prediction_features(stock_info,predictions):
        data = {}
        for key,val in stock_info.items():
                new_data = pd.DataFrame()
                print(np.reshape(predictions[key],502))
                predictive_series = pd.Series(np.reshape(predictions[key],502))
                new_data['open'] = val['open']
                new_data['close_truth'] = val['close']
                new_data['close_predictive'] = predictive_series
                print(predictive_series)
                print(new_data)
                

#stocks being tracked:
#       home depot,             nike,           johnson & johnson, visa,              
#       disney,                 mcdonalds,      IBM,               chevron,
#       microsoft,              intel,          united health,     caterpillar,
#       boeing,                 exon,           proctor & gamble   merck,
#       minisota minning,       goldman,        general electric,  walmart,
#       pfizer,                 jp morgan,      cisco,             verizon,
#       united tec,             travlers,       apple,             cisco,

stock_names = {
        'HD':'EOD/HD',     'DIS':'EOD/DIS',   'MSFT':'EOD/MSFT',  'BA':'EOD/BA',
        'MMM':'EOD/MMM',   'PFE':'EOD/PFE',   'NKE':'EOD/NKE',    'JNJ':'EOD/JNJ',
        'MCD':'EOD/MCD',   'INTC':'EOD/INTC', 'XOM':'EOD/XOM',    'GS':'EOD/GS',
        'JPM':'EOD/JPM',   'AXP':'EOD/AXP',   'V':'EOD/V',        'IBM':'EOD/IBM',
        'UNH':'EOD/UNH',   'PG':'EOD/PG',     'GE':'EOD/GE',      'KO':'EOD/KO',
        # 'CSCO':'EOD/CSCO', 'CVX':'EOD/CVX',   'CAT':'EOD/CAT',    'MRK':'EOD/MRK',
        # 'WMT':'EOD/WMT',   'VZ':'EOD/VZ',     'UTX':'EOD/UTX',    'TRV':'EOD/TRV',
        # 'AAPL':'EOD/AAPL'
        }
start = '2013-09-01'   
end = '2017-12-28'
start_train = datetime.datetime(2013 ,9 , 1 , 0 , 0)
end_train = datetime.datetime(2015,12,31,0,0)
start_test = datetime.datetime(2016,1,1,0,0)
end_test = datetime.datetime(2017,12,28,0,0)
stock_info = set_stocks(stock_names,start,end)
training_data = training_sets(stock_info,start_train,end_train)
test_data = test_sets(stock_info,start_test,end_test)
linear_regression = predictions(stock_info,training_data,test_data)     
stock_info = prediction_features(stock_info,linear_regression)
graph(linear_regression,test_data)
