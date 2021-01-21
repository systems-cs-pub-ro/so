from datetime import date

class CommentHeader:
	def set(self, lab_no=0,
				lab_name='', ex_no=0, platform='Linux', comment=''):
		self.lab_no = lab_no
		self.lab_name = lab_name
		self.ex_no = ex_no
		self.platform = platform
		self.comment = comment

	def to_string(self):
		text  = '/**\n'
		text += '  * SO, ' + str(date.today().year) + '\n'
		text += '  * Lab #' + str(self.lab_no) + '\n'
		text += '  *\n'
		text += '  * Task #' + str(self.ex_no) + ', ' \
							 + self.platform + '\n'
		text += '  *\n'

		if (len(self.comment) > 0):
			"""Adding actual comment - on multiple lines if necessary."""
			text += '  * ' + self.comment + '\n'
		text += '  */\n'

		return text
