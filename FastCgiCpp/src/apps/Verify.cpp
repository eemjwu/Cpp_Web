#include "Verify.h"

Response Verify::get_verify(Request req)
{
	DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
		req.getMethod().c_str(),
		req.getUrl().c_str(),
		req.getParams().c_str(),
		req.getCookie().c_str());
	Params reqParams(req.getParams());
	Cookie reqCookie(req.getCookie());
	Response res;

	const int gifsize = 17646;
	unsigned char l[6];
	unsigned char im[70 * 200];
	unsigned char gif[gifsize];
	captcha(im, l);
	makegif(im, gif);

	// ���ͼƬ��
	res.setByteData(reinterpret_cast<char *>(gif));
	res.setContentType("image/gif");
	res.setSetCookie(reqCookie);

	// ���ַ���ת���ɴ�д���浽session��
	char * v = reinterpret_cast<char *>(l);
	TString verify_code = static_cast<std::string>(v);
	verify_code.toUpper();
	MySession->setVerify(verify_code);

	DBG(L_DEBUG, "��֤��: %s", l);
	return res;
}
