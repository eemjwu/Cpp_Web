#include "web_helper.h"

namespace WebTool
{
	std::string web_helper::return_msg(const int& state, const std::string& msg, const std::string& data_json)
	{
		std::string res_json;

		TJson::setInt(res_json, "state", state);
		TJson::setStr(res_json, "msg", msg);
		TJson::setObj(res_json, "data", data_json);

		return res_json;
	}
	char*  web_helper::urldecode( char* encd, char* decd)
	{
		int j, i;
		char *cd = (char*)encd;
		char p[2];
		unsigned int num;
		j = 0;

		for (i = 0; i < strlen(cd) ; i++)
		{
			memset(p, '/0', 2);
			if (cd[i] != '%')
			{
				decd[j++] = cd[i];
				continue;
			}

			p[0] = cd[++i];
			p[1] = cd[++i];

			p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);
			p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);
			decd[j++] = (p[0] * 16 + p[1]);

		}
		decd[j] = '\0';

		DBG(L_DEBUG, "����:%s", decd);
		return decd;
	}
	int web_helper::upload_file_save_as(FCGX_Request& request, TString& resfilename, const char * path, const char * newname)//����˵��  ������{ ·�������ļ����������� } ����{ �ϴ����ļ��� } ���أ�������
	{
		FILE *fp;
		int getState = STATE_START;
		int contentLength; /* uploaded file content length */
		int nowReadLen;
		int signCodeLen;
		int tmpLen;
		char *nowReadP;
		char *nowWriteP;
		char dealBuf[DEAL_BUF_LEN];
		char signCode[SIGN_CODE_LEN]; /* http boundary */
		char tmpSignCode[SIGN_CODE_LEN];
		char fileName[FILE_NAME_LEN];
		memset(dealBuf, 0, DEAL_BUF_LEN);
		memset(signCode, 0, SIGN_CODE_LEN);
		memset(fileName, 0, FILE_NAME_LEN);
		nowReadLen = 0;
		if (FCGX_GetParam("CONTENT_LENGTH", request.envp))
		{
			contentLength = atoi(FCGX_GetParam("CONTENT_LENGTH", request.envp));
			DBG(L_INFO, "CONTENT_LENGTH:%d", contentLength);
		}
		else
		{
			DBG(L_INFO, "CONTENT_LENGTH:%d", contentLength);
			return EUPLOAD_NO_DATA;
		}

		while (contentLength > 0)
		{
			if (contentLength >= DEAL_BUF_LEN)
			{
				nowReadLen = DEAL_BUF_LEN;
			}
			else
			{
				nowReadLen = contentLength;
			}
			contentLength -= nowReadLen;

			//if (fread(dealBuf, sizeof(char), nowReadLen, FCGI_stdin) != nowReadLen)
			if ((FCGX_GetStr(dealBuf, nowReadLen, request.in)) != nowReadLen)
			{
				DBG(L_INFO, "read error %d", nowReadLen);
				//log_error("read error %d", nowReadLen);
				return EUPLOAD_READ;
			}
			nowReadP = dealBuf;
			while (nowReadLen > 0)
			{
				switch (getState)
				{
				case STATE_START:
					nowWriteP = signCode;
					getState = STATE_GET_SIGN_CODE;
				case STATE_GET_SIGN_CODE:
					if (strncmp(nowReadP, "\r\n", 2) == 0)
					{
						signCodeLen = nowWriteP - signCode;
						nowReadP++;
						nowReadLen--;
						*nowWriteP = 0;
						getState = STATE_GET_FILE_NAME;
					}
					else
					{
						*nowWriteP = *nowReadP;
						nowWriteP++;
					}
					break;
				case STATE_GET_FILE_NAME:
					if (strncmp(nowReadP, "filename=", strlen("filename=")) == 0)
					{

						nowReadP += strlen("filename=");
						nowReadLen -= strlen("filename=");
						nowWriteP = fileName + strlen(path);//ָ��fileName�����е� strlen(path)Ԫ�ص�λ��	
						while (*nowReadP != '\r')
						{
							if (*nowReadP == '\\' || *nowReadP == '/')
							{
								nowWriteP = fileName + strlen(path);
							}
							else if (*nowReadP != '\"')
							{
								*nowWriteP = *nowReadP;
								nowWriteP++;
							}
							nowReadP++;
							nowReadLen--;
						}
						*nowWriteP = 0;
						nowReadP++;
						nowReadLen--;
						getState = STATE_GET_FILE_START;
						DBG(L_INFO, "127");
						/////////////////////////�Һ�׺///////////////////////////////
						resfilename = TString(fileName + strlen(path));
						DBG(L_INFO, "resfilename:%s", resfilename.c_str());
						size_t pos = resfilename.find_first_of(".");
						TString ex = resfilename.right(resfilename.length() - pos - 1);
						DBG(L_INFO, "ex:%s", ex.c_str());
						
						if (strcmp(newname, "") == 0)//���û�д��������ļ���
						{
							memcpy(fileName, path, strlen(path));
							if ((fp = fopen(fileName, "w")) == NULL)
							{
								DBG(L_INFO, "open file %s error %d", fileName, errno);
								//log_error("open file %s error %d", fileName, errno);
								return EUPLOAD_WRITE;
							}
						}
						else
						{
							char *result = (char*)malloc(strlen(path) + strlen(newname)  + strlen(ex.c_str()) + 1);
							if (result == NULL)
							{
								DBG(L_INFO, "�ڴ����ʧ��");
								exit(1); //Ӧ��Ҫtryһ��  ��Ϊ�����ڴ治��
							}
							strcpy(result, path);
							strcat(result, newname);
							
							std::string pname = static_cast <std::string>(result) + "." + ex;
							DBG(L_INFO, "pname:%s", pname.c_str());
							if ((fp = fopen(pname.c_str(), "w")) == NULL)
							{
								DBG(L_INFO, "open file %s error %d", fileName, errno);
								//log_error("open file %s error %d", fileName, errno);
								free(result);
								return EUPLOAD_WRITE;
							}
							DBG(L_INFO, "result:%s", result);
							free(result);
						}

					}
					break;
				case STATE_GET_FILE_START:
					if (strncmp(nowReadP, "\r\n\r\n", 4) == 0)
					{
						nowReadP += 3;
						nowReadLen -= 3;
						getState = STATE_GET_FILE_CONTENT;
					}
					break;
				case STATE_GET_FILE_CONTENT:
					if (*nowReadP != '\r')
					{
						fputc(*nowReadP, fp);
					}
					else
					{
						if (nowReadLen >= (signCodeLen + 2)) //\r\n=2
						{
							if (strncmp(nowReadP + 2, signCode, signCodeLen) == 0)
							{
								getState = STATE_END;
								nowReadLen = 1;
							}
							else
							{
								fputc(*nowReadP, fp);
							}
						}
						else
						{
							getState = STATE_CHECK_END;
							nowWriteP = tmpSignCode;
							*nowWriteP = *nowReadP;
							nowWriteP++;
							tmpLen = 1;
						}
					}
					break;
				case STATE_CHECK_END:
					if (*nowReadP != '\r')
					{
						if (tmpLen < signCodeLen + 2)
						{
							*nowWriteP = *nowReadP;
							nowWriteP++;
							tmpLen++;
							if (tmpLen == signCodeLen + 2)
							{
								*nowWriteP = 0;
								if ((tmpSignCode[1] == '\n') && (strncmp(tmpSignCode + 2, signCode, signCodeLen) == 0))
								{
									getState = STATE_END;
									nowReadLen = 1;
								}
								else
								{
									//fprintf(fp,tmpSignCode);
									fwrite(tmpSignCode, sizeof(char), tmpLen, fp);
									getState = STATE_GET_FILE_CONTENT;
								}
							}
						}
					}
					else
					{
						*nowWriteP = 0;
						//fprintf(fp,tmpSignCode);
						fwrite(tmpSignCode, sizeof(char), tmpLen, fp);
						nowWriteP = tmpSignCode;
						*nowWriteP = *nowReadP;
						nowWriteP++;
						tmpLen = 1;
					}
					break;
				case STATE_END:
					nowReadLen = 1;
					break;
				default:break;
				}
				nowReadLen--;
				nowReadP++;
			}
		}
		if (fp != NULL)
		{
			fclose(fp);
		}
		return EUPLOAD_SUCCESS;
	}

	//	while (contentLength > 0)
	//	{
	//		if (contentLength >= DEAL_BUF_LEN)
	//		{
	//			nowReadLen = DEAL_BUF_LEN;
	//		}
	//		else
	//		{
	//			nowReadLen = contentLength;
	//		}
	//		contentLength -= nowReadLen;
	//		if (fread(dealBuf, sizeof(char), nowReadLen, FCGI_stdin) != (size_t)nowReadLen)
	//		{
	//			//log_error("read error %d", nowReadLen);
	//			return EUPLOAD_READ;
	//		}
	//		nowReadP = dealBuf;
	//		while (nowReadLen > 0)
	//		{
	//			switch (getState)
	//			{
	//			case STATE_START:
	//				nowWriteP = signCode;
	//				getState = STATE_GET_SIGN_CODE;
	//			case STATE_GET_SIGN_CODE:
	//				if (strncmp(nowReadP, "\r\n", 2) == 0)
	//				{
	//					signCodeLen = nowWriteP - signCode;
	//					nowReadP++;
	//					nowReadLen--;
	//					*nowWriteP = 0;
	//					getState = STATE_GET_FILE_NAME;
	//				}
	//				else
	//				{
	//					*nowWriteP = *nowReadP;
	//					nowWriteP++;
	//				}
	//				break;
	//			case STATE_GET_FILE_NAME:
	//				if (strncmp(nowReadP, "filename=", strlen("filename=")) == 0)
	//				{
	//					nowReadP += strlen("filename=");
	//					nowReadLen -= strlen("filename=");
	//					nowWriteP = fileName + strlen(saveas);
	//					while (*nowReadP != '\r')
	//					{
	//						if (*nowReadP == '\\' || *nowReadP == '/')
	//						{
	//							nowWriteP = fileName + strlen(saveas);
	//						}
	//						else if (*nowReadP != '\"')
	//						{
	//							*nowWriteP = *nowReadP;
	//							nowWriteP++;
	//						}
	//						nowReadP++;
	//						nowReadLen--;
	//					}
	//					*nowWriteP = 0;
	//					nowReadP++;
	//					nowReadLen--;
	//					getState = STATE_GET_FILE_START;
	//					memcpy(fileName, saveas, strlen(saveas));
	//					if ((fp = fopen(fileName, "w")) == NULL)
	//					{
	//						//log_error("open file %s error %d", fileName, errno);
	//						return EUPLOAD_WRITE;
	//					}
	//				}
	//				break;
	//			case STATE_GET_FILE_START:
	//				if (strncmp(nowReadP, "\r\n\r\n", 4) == 0)
	//				{
	//					nowReadP += 3;
	//					nowReadLen -= 3;
	//					getState = STATE_GET_FILE_CONTENT;
	//				}
	//				break;
	//			case STATE_GET_FILE_CONTENT:
	//				if (*nowReadP != '\r')
	//				{
	//					fputc(*nowReadP, fp);
	//				}
	//				else
	//				{
	//					if (nowReadLen >= (signCodeLen + 2)) //\r\n=2
	//					{
	//						if (strncmp(nowReadP + 2, signCode, signCodeLen) == 0)
	//						{
	//							getState = STATE_END;
	//							nowReadLen = 1;
	//						}
	//						else
	//						{
	//							fputc(*nowReadP, fp);
	//						}
	//					}
	//					else
	//					{
	//						getState = STATE_CHECK_END;
	//						nowWriteP = tmpSignCode;
	//						*nowWriteP = *nowReadP;
	//						nowWriteP++;
	//						tmpLen = 1;
	//					}
	//				}
	//				break;
	//			case STATE_CHECK_END:
	//				if (*nowReadP != '\r')
	//				{
	//					if (tmpLen < signCodeLen + 2)
	//					{
	//						*nowWriteP = *nowReadP;
	//						nowWriteP++;
	//						tmpLen++;
	//						if (tmpLen == signCodeLen + 2)
	//						{
	//							*nowWriteP = 0;
	//							if ((tmpSignCode[1] == '\n') && (strncmp(tmpSignCode + 2, signCode, signCodeLen) == 0))
	//							{
	//								getState = STATE_END;
	//								nowReadLen = 1;
	//							}
	//							else
	//							{
	//								//fprintf(fp,tmpSignCode);
	//								fwrite(tmpSignCode, sizeof(char), tmpLen, fp);
	//								getState = STATE_GET_FILE_CONTENT;
	//							}
	//						}
	//					}
	//				}
	//				else
	//				{
	//					*nowWriteP = 0;
	//					//fprintf(fp,tmpSignCode);
	//					fwrite(tmpSignCode, sizeof(char), tmpLen, fp);
	//					nowWriteP = tmpSignCode;
	//					*nowWriteP = *nowReadP;
	//					nowWriteP++;
	//					tmpLen = 1;
	//				}
	//				break;
	//			case STATE_END:
	//				nowReadLen = 1;
	//				break;
	//			default:break;
	//			}
	//			nowReadLen--;
	//			nowReadP++;
	//		}
	//	}
	//	if (fp != NULL)
	//	{
	//		fclose(fp);
	//	}
	//	return EUPLOAD_SUCCESS;
	//}
}

